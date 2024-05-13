package HttpServer;

import java.io.*;
import java.net.Socket;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class ServerThread extends Thread {
    private Socket connection;
    private int MAX_BUFFER_SIZE=10000000; // in bytes
    private int MIN_CHUNK_SIZE=1000; // in bytes
    private int MAX_CHUNK_SIZE=200; // in bytes


    public static List<String>LoggedUsers=new ArrayList<String>();          // who were logged in at least once
    public static List<String>OnlineUsers=new ArrayList<String>();          // who are currently active

    public static List<UploadedFile>privateFiles=new ArrayList<UploadedFile>();   // all private files uploaded by users
    public static List<UploadedFile>publicFiles=new ArrayList<UploadedFile>();    // all public files uploaded by users

    public static List<Request>requestList=new ArrayList<Request>();         // all the file requests
    public static List<Message>messageList=new ArrayList<Message>();         // storing all the unread messages to send once

    public ServerThread(Socket connection) {
        this.connection = connection;
    }

    public void run()
    {
        try {
            ObjectOutputStream out = new ObjectOutputStream(this.connection.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(this.connection.getInputStream());

            out.writeObject("Please Enter your userName");
            String userName;
            userName = (String) in.readObject();
            File baseDir = new File("");
            if (checkValidity(userName))
            {
                out.writeObject("Success Login");
                if(ServerThread.LoggedUsers.contains(userName)==false)
                    ServerThread.LoggedUsers.add(userName);     // added for the first time
                ServerThread.OnlineUsers.add(userName);         // the user is in active list
                String uploadPath = baseDir.getAbsolutePath().concat("/uploaded/" + userName);
                File uploadDirectory = new File(uploadPath);
                if (!uploadDirectory.exists()) {
                    boolean res = uploadDirectory.mkdir();
                    System.out.println("upload path " + uploadPath + " " + res+" created for the client named "+userName);
                }

                while (true)
                {
                    String option = (String) in.readObject();
                    System.out.println("Option: "+option);

                     if (option.equalsIgnoreCase("7"))
                    {
                        ServerThread.OnlineUsers.remove(userName);
                        System.out.println("logged out " + userName);
                        connection.close();
                         break;
                     }
                    else if (option.equalsIgnoreCase("1"))
                    {
                         int totalUsers = ServerThread.LoggedUsers.size();
                         out.writeObject(String.valueOf(totalUsers));
                         for (int i = 1; i <= totalUsers; i++)
                         {
                            String status = "Offline";
                            if (ServerThread.OnlineUsers.contains(ServerThread.LoggedUsers.get(i - 1))) {
                             status = "Online";
                             }
                             String now = String.valueOf(i) + "," + status + "," + ServerThread.LoggedUsers.get(i - 1);
                             out.writeObject(now);
                         }
                     }
                else if (option.equalsIgnoreCase("2")) // Viewing own files
                {
                    List<UploadedFile> showPrivateFiles = new ArrayList<UploadedFile>();
                    List<UploadedFile> showPublicFiles = new ArrayList<UploadedFile>();
                    List<UploadedFile> showAllFiles = new ArrayList<UploadedFile>();
                    for (int i = 0; i < ServerThread.publicFiles.size(); i++) {
                        UploadedFile nowFile = ServerThread.publicFiles.get(i);
                        if (nowFile.owner.equalsIgnoreCase(userName)) {
                            showPublicFiles.add(nowFile);
                            showAllFiles.add(nowFile);
                        }
                    }
                    int start=1;
                    for (int i = 0; i < ServerThread.privateFiles.size(); i++) {
                        UploadedFile nowFile = ServerThread.privateFiles.get(i);
                        if (nowFile.owner.equalsIgnoreCase(userName)) {
                            showPrivateFiles.add(nowFile);
                            showAllFiles.add(nowFile);
                        }
                    }
                    out.writeObject(String.valueOf(showPrivateFiles.size() + showPublicFiles.size()));
                    for (int i = 0; i < showPublicFiles.size(); i++) {
                        UploadedFile nowFile = showPublicFiles.get(i);
                        String msg=String.valueOf(start) + ", "+String.valueOf(nowFile.fileId)+ ", "+nowFile.fileName+","+nowFile.owner+ ", "+nowFile.status +", "+String.valueOf(nowFile.size)
                                + ", "+nowFile.date.toString()+", "+nowFile.URL;
                        out.writeObject(msg);
                        start++;
                    }
                    for (int i = 0; i < showPrivateFiles.size(); i++) {
                        UploadedFile nowFile = showPrivateFiles.get(i);
                        String msg=String.valueOf(start) + ", "+String.valueOf(nowFile.fileId)+ ", "+nowFile.fileName+","+nowFile.owner+ ", "+nowFile.status +", "+String.valueOf(nowFile.size)
                                + ", "+nowFile.date.toString()+", "+nowFile.URL;
                        out.writeObject(msg);
                        start++;
                    }
                    int choice = Integer.parseInt((String) in.readObject());
                    if (choice == 2) {
                        // do nothing
                    } else {
                        System.out.println("The client wants to download some files");
                        int idxFile = Integer.parseInt((String) in.readObject());
                        idxFile = idxFile - 1;
                        UploadedFile nowFile = showAllFiles.get(idxFile);
                        String path = nowFile.URL;

                        File file = new File(path);
                        if (!file.exists()) {
                            System.out.println("File not found");
                            return;
                        }

                        FileInputStream fileInputStream = new FileInputStream(file);
                        out.flush();

                        // Here we  break file into chunks
                        byte[] buffer = new byte[MAX_CHUNK_SIZE];
                        out.writeObject(String.valueOf(MAX_CHUNK_SIZE));

                        int bytes = 0;
                        while ((bytes = fileInputStream.read(buffer)) != -1) {
                            out.write(buffer, 0, bytes);
                            out.flush();
                        }
                        System.out.println("File sent to client");
                        out.writeObject("Ended");
                        fileInputStream.close();
                        out.flush();
                    }


                }
                else if (option.equalsIgnoreCase("3"))
                {
                    List<UploadedFile> showAllFiles = new ArrayList<UploadedFile>();
                    for (int i = 0; i < ServerThread.publicFiles.size(); i++) {
                        UploadedFile nowFile = ServerThread.publicFiles.get(i);
                        if(nowFile.owner.equalsIgnoreCase(userName)==false)
                            showAllFiles.add(nowFile); // see others public files rather than own files
                    }

                    out.writeObject(String.valueOf(showAllFiles.size()));

                    for (int i = 0; i < showAllFiles.size(); i++) {
                        UploadedFile nowFile = showAllFiles.get(i);
                        String msg=String.valueOf(i+1) + ", "+String.valueOf(nowFile.fileId)+ ", "+nowFile.fileName+","+nowFile.owner+ ", "+nowFile.status +", "+String.valueOf(nowFile.size)
                                + ", "+nowFile.date.toString()+", "+nowFile.URL;
                        out.writeObject(msg);
                    }

                    int choice = Integer.parseInt((String) in.readObject());
                    if (choice == 2) {
                        // do nothing
                    } else {
                        System.out.println("The client wants to download some files");
                        int idxFile = Integer.parseInt((String) in.readObject());
                        idxFile = idxFile - 1;
                        UploadedFile nowFile = showAllFiles.get(idxFile);
                        String path = nowFile.URL;

                        File file = new File(path);
                        if (!file.exists()) {
                            System.out.println("File not found");
                            return;
                        }

                        FileInputStream fileInputStream = new FileInputStream(file);
                        out.flush();

                        // Here we  break file into chunks

                        byte[] buffer = new byte[MAX_CHUNK_SIZE];
                        out.writeObject(String.valueOf(MAX_CHUNK_SIZE));

                        int bytes = 0;
                        while ((bytes = fileInputStream.read(buffer)) != -1) {
                            out.write(buffer, 0, bytes);
                            out.flush();
                        }

                        System.out.println("File sent to client");
                        out.writeObject("o");
                        fileInputStream.close();
                        out.flush();

                    }


                }
                else if (option.equalsIgnoreCase("5"))
                {
                    String fileName = (String) in.readObject();
                    System.out.println(fileName);
                    String fileStatusInt = (String) in.readObject();
                    System.out.println(fileStatusInt);
                    int fileSize = Integer.parseInt((String) in.readObject());
                    System.out.println(fileName + " " + fileStatusInt + " " + fileSize);
                    String fileStatus = "";
                    if (fileStatusInt.equalsIgnoreCase("1"))
                        fileStatus = "Private";
                    else
                        fileStatus = "Public";

                    if (uploadPossible(fileSize) == true) {
                        out.writeObject("Possible");
                        int chunk_size = (int) Math.floor(Math.random() * (MAX_CHUNK_SIZE - MIN_CHUNK_SIZE + 1) + MIN_CHUNK_SIZE);
                        int fileId = (ServerThread.privateFiles.size() + ServerThread.publicFiles.size()) + 1;

                        out.writeObject((String.valueOf(chunk_size)));
                        out.writeObject(String.valueOf(fileId));
                        out.flush();

                        int bytesRead = 0;
                        byte[] buffer = new byte[chunk_size];


                        String basePath = new File("").getAbsolutePath();
                        String path = basePath.concat("/uploaded/" + userName + "/" + fileName);
                        FileOutputStream fileOutputStream = null;
                        try {
                            fileOutputStream = new FileOutputStream(path);
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        }
                        boolean gotTheFile=true;
                        try {
                            long size = 0;
                            while ((bytesRead = in.read(buffer, 0, buffer.length)) != -1) {
                                String statusMsg = (String) in.readObject();
                                //Thread.sleep(7000);
                                out.writeObject("got");
                                String feedback = (String) in.readObject();
                                if (feedback.equalsIgnoreCase("timeout")) {
                                    gotTheFile=false;
                                    break;
                                }
                                fileOutputStream.write(buffer, 0, bytesRead);
                                size += bytesRead;
                            }
                            if(size!=fileSize || gotTheFile==false)
                            {
                                File f=new File(path);
                                f.delete();
                                gotTheFile=false;
                            }

                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        /*catch (InterruptedException e) {
                            e.printStackTrace();
                        }*/

                        // Comment for timeout

                        fileOutputStream.close();
                        if(gotTheFile==true)
                             System.out.println("File uploaded successfully");
                        String statusMsg=(String)in.readObject();
                        System.out.println(statusMsg);

                        if (gotTheFile==true && fileStatusInt.equalsIgnoreCase("1")) {
                            UploadedFile currFile = new UploadedFile(fileName,(ServerThread.privateFiles.size() + ServerThread.publicFiles.size() +1), fileSize, userName, fileStatus, path, LocalDate.now());
                            ServerThread.privateFiles.add(currFile);
                        } else if(gotTheFile==true){
                            UploadedFile currFile = new UploadedFile(fileName,(ServerThread.privateFiles.size() + ServerThread.publicFiles.size() +1), fileSize, userName, fileStatus, path, LocalDate.now());
                            ServerThread.publicFiles.add(currFile);
                        }

                        if(fileStatus.equalsIgnoreCase("private") || gotTheFile==false)
                            continue;


                        int reqOption=Integer.parseInt((String)in.readObject());
                        if(reqOption==1 && gotTheFile==true)
                        {
                            int totalReqs=ServerThread.requestList.size();
                            out.writeObject((String.valueOf(totalReqs)));
                            for(int i=0;i<ServerThread.requestList.size();i++)
                            {
                                Request nowReq=ServerThread.requestList.get(i);
                                String now=Integer.toString(i+1);
                                out.writeObject("Req ID: "+now+" Request made by: "+nowReq.senderName);
                                out.writeObject("Description: "+nowReq.description);
                            }
                            int reqIDHandler=Integer.parseInt((String) in.readObject());
                            reqIDHandler--;
                            Message message=new Message();
                            message.sender=userName;
                            message.description=userName+ " has uploaded a file in response to your request ID: "+Integer.toString(reqIDHandler+1);
                            message.receiver = ServerThread.requestList.get(reqIDHandler).senderName;
                            ServerThread.messageList.add(message);

                        }
                        else
                        {
                            continue;
                        }



                    }
                    else
                        {
                        out.writeObject("Impossible");
                    }

                } else if (option.equalsIgnoreCase("6")) {
                    String desc = (String) in.readObject();
                    int id = ServerThread.requestList.size() + 1;
                    Request request = new Request(id, desc, userName, "");
                    out.writeObject(String.valueOf(id));
                    ServerThread.requestList.add(request);
                    // generate msgs
                    for (int i = 0; i < ServerThread.LoggedUsers.size(); i++)
                    {
                        if(ServerThread.LoggedUsers.get(i).equalsIgnoreCase(userName))
                            continue;
                        Message message = new Message();
                        message.receiver = ServerThread.LoggedUsers.get(i);
                        message.description = userName + " has requested for a file with the following description.\n\"" + desc + " \"\n";
                        message.sender = userName;
                        ServerThread.messageList.add(message);
                        System.out.println("added in messgae List");
                    }
                } else if (option.equalsIgnoreCase("4")) {
                    int totalCount = 0;
                    System.out.println(userName + " is trying to see msg");
                    for (int i = 0; i < ServerThread.messageList.size(); i++) {
                        Message message = ServerThread.messageList.get(i);
                        if (message.receiver.equalsIgnoreCase(userName)) {
                            totalCount++;
                        }
                    }
                    List<Message> to_be_removed = new ArrayList<Message>();
                    out.writeObject(String.valueOf(totalCount));
                    for (int i = 0; i < ServerThread.messageList.size(); i++) {
                        Message message = ServerThread.messageList.get(i);
                        if (message.receiver.equalsIgnoreCase(userName)) {
                            out.writeObject(message.sender);
                            out.writeObject(message.description);
                            to_be_removed.add(message);
                        }
                    }
                    for (int i = 0; i < to_be_removed.size(); i++) {
                        ServerThread.messageList.remove(to_be_removed.get(i));
                    }
                } else {
                    System.out.println("Please Enter correct option");
                }
            }
        } else {
                out.writeObject("Unsuccessful Login due to same username, Connection disrupted.\n Try another username.");
                connection.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e)
        {
            e.printStackTrace();
        }
    }

    private boolean uploadPossible(int fileSize) {
        boolean possible=true;
        int total=0;
        for(int i=0;i<privateFiles.size();i++)
        {
            int nowSize=privateFiles.get(i).size;
            total+=nowSize;
        }
        for(int i=0;i<publicFiles.size();i++)
        {
            int nowSize=publicFiles.get(i).size;
            total+=nowSize;
        }
        total+=fileSize;
        if(total>MAX_BUFFER_SIZE) // buffer size exceeded
        {
            possible=false;
        }
        return possible;
    }

    private boolean checkValidity(String userName) {
        boolean ok=true;
        if(ServerThread.OnlineUsers.contains(userName))
        {
            ok=false;
        }
        return ok;
    }
}

