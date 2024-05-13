package HttpClient;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.Scanner;
import java.lang.*;

public class Client {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        Socket socket = new Socket("localhost", 5022);
        System.out.println("Connection established");
        System.out.println("Remote port: " + socket.getPort());
        System.out.println("Local port: " + socket.getLocalPort());

        // buffers
        ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
        ObjectInputStream in = new ObjectInputStream(socket.getInputStream());

        Scanner scanner = new Scanner(System.in);
        String msg = (String) in.readObject();
        System.out.println(msg);
        String userName = scanner.nextLine();
        out.writeObject(userName);
        msg = (String) in.readObject();

        if(msg.equalsIgnoreCase("Success Login"))
        {
            System.out.println(msg);
            while(true)
            {
                System.out.println("Enter an option-------------");
                System.out.println("");
                System.out.println("1. View Clients List");
                System.out.println("2. Look up my files");
                System.out.println("3. View others files");
                System.out.println("4. See Unread Messages");
                System.out.println("5. Upload a file");
                System.out.println("6. Request for a file");
                System.out.println("7. Log out");
                System.out.println("");
                String option = scanner.nextLine();
                if (option.equalsIgnoreCase("7"))
                {
                    out.writeObject(option);
                    System.out.println("Logging Out.............");
                    socket.close();
                    break;
                }
                else if(option.equalsIgnoreCase("2"))
                {
                    out.writeObject(option);
                    int totalFiles = Integer.parseInt((String)in.readObject());
                    System.out.println("There are total "+totalFiles+" files in your directory.");
                    System.out.println("The list is shown below--------");
                    System.out.println("");
                    String fileDetails="";
                    String res2[]={"No", "FileID", "Filename","Owner","Status","File_size","Upload_Date","URL"};
                        for(int j=0;j<res2.length;j++)
                        {
                            if(j==0 || j==1)
                            {
                                System.out.print(String.format("%" + 10 + "s", res2[j]));
                            }
                            else if(j==2 || j==3)
                            {
                                System.out.print(String.format("%" + 20 + "s", res2[j]));
                            }
                            else if(j==4 || j==5)
                            {
                                System.out.print(String.format("%" + 20 + "s", res2[j]));
                            }
                            else
                            {
                                System.out.print(String.format("%" + 35 + "s", res2[j]));
                            }
                        }
                        System.out.println("");

                    for(int i=1;i<=totalFiles;i++)
                    {
                        fileDetails = (String)in.readObject();
                        String[] res = fileDetails.split("[,]", 0);
                        for(int j=0;j<res.length;j++)
                        {
                            if(j==0 || j==1)
                            {
                                System.out.print(String.format("%" + 10 + "s", res[j]));
                            }
                            else if(j==2 || j==3 || j==4)
                            {
                                System.out.print(String.format("%" + 20 + "s", res[j]));
                            }
                            else if(j==4 || j==5)
                            {
                                System.out.print(String.format("%" + 20 + "s", res[j]));
                            }
                            else
                            {
                                System.out.print(String.format("%" + 35 + "s", res[j]));
                            }
                        }
                        System.out.println("");
                    }
                    System.out.println("");

                    System.out.println("Do you want to download any file?");
                    System.out.println("1. YES");
                    System.out.println("2. NO");
                    int choice=Integer.parseInt(scanner.nextLine());
                    if(choice==1)
                    {

                        File baseDir = new File("");
                        String uploadPath = baseDir.getAbsolutePath().concat("/hehe/"+userName);
                        System.out.println("Please Enter the path where you want to download the file");
                        uploadPath=scanner.nextLine();
                        File uploadDirectory = new File(uploadPath);
                        if( !uploadDirectory.exists() ) {
                            System.out.println(uploadPath);
                            boolean res=uploadDirectory.mkdir();
                        }
                        String filename="";

                        System.out.println("Enter the serial No. of the file");
                        int indexOfFile = Integer.parseInt(scanner.nextLine());

                        System.out.println("Enter the file name that you want to save");
                        filename=scanner.nextLine();
                        out.writeObject(String.valueOf(choice));

                        out.writeObject((String.valueOf(indexOfFile)));

                        out.flush();

                        int bytesRead = 0;
                        int chunk_size=Integer.parseInt((String) in.readObject());
                        System.out.println("The size of the chunk is "+chunk_size);
                        byte[] buffer = new byte[chunk_size];
                        String basePath = new File("").getAbsolutePath();
                        String path ="";
                        path=uploadPath+filename;
                        FileOutputStream fileOutputStream = null;
                        try {
                            fileOutputStream = new FileOutputStream(path);
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        }
                        try{
                            long size = 0;
                            while((bytesRead=in.read(buffer, 0, buffer.length))!=-1) {
                                fileOutputStream.write(buffer, 0, bytesRead);
                                size += bytesRead;
                            }
                        }catch (IOException e)
                        {
                            e.printStackTrace();
                        }
                        fileOutputStream.close();
                        String here=(String)in.readObject();
                        System.out.println(here);
                        out.flush();
                        System.out.println("File downloaded successfully");
                    }
                    else
                    {
                        out.writeObject(String.valueOf(choice));
                    }

                }
                else if(option.equalsIgnoreCase("3"))
                {
                    out.writeObject(option);
                    int totalFiles = Integer.parseInt((String)in.readObject());
                    System.out.println("There are total "+totalFiles+" files in the public directory.");
                    System.out.println("The list is shown below--------");
                    System.out.println("");
                    String fileDetails="";
                    String res2[]={"No", "FileID", "Filename","Owner","Status","Filesize","Upload_Date","URL"};
                    for(int j=0;j<res2.length;j++)
                    {
                        if(j==0 || j==1)
                        {
                            System.out.print(String.format("%" + 10 + "s", res2[j]));
                        }
                        else if(j==2 || j==3)
                        {
                            System.out.print(String.format("%" + 20 + "s", res2[j]));
                        }
                        else if(j==4 || j==5)
                        {
                            System.out.print(String.format("%" + 20 + "s", res2[j]));
                        }
                        else
                        {
                            System.out.print(String.format("%" + 35 + "s", res2[j]));
                        }
                    }
                    System.out.println("");

                    for(int i=1;i<=totalFiles;i++)
                    {
                        fileDetails = (String)in.readObject();
                        String[] res = fileDetails.split("[,]", 0);
                        for(int j=0;j<res.length;j++)
                        {
                            if(j==0 || j==1)
                            {
                                System.out.print(String.format("%" + 10 + "s", res[j]));
                            }
                            else if(j==2 || j==3 || j==4)
                            {
                                System.out.print(String.format("%" + 20 + "s", res[j]));
                            }
                            else if(j==4 || j==5)
                            {
                                System.out.print(String.format("%" + 20 + "s", res[j]));
                            }
                            else
                            {
                                System.out.print(String.format("%" + 35 + "s", res[j]));
                            }
                        }
                        System.out.println("");
                    }
                    System.out.println("");
                    System.out.println("Do you want to download any file?");
                    System.out.println("1. YES");
                    System.out.println("2. NO");
                    int choice=Integer.parseInt(scanner.nextLine());
                    if(choice==1)
                    {

                        File baseDir = new File("");
                        String uploadPath = baseDir.getAbsolutePath().concat("/hehe/"+userName);
                        System.out.println("Please Enter the path where you want to download the file");
                        uploadPath=scanner.nextLine();
                        File uploadDirectory = new File(uploadPath);
                        if( !uploadDirectory.exists() ) {
                            System.out.println(uploadPath);
                            boolean res=uploadDirectory.mkdir();
                        }
                        String filename="";

                        System.out.println("Enter the Serial No. of the file");
                        int indexOfFile = Integer.parseInt(scanner.nextLine());

                        System.out.println("Enter the file name that you want to save");
                        filename=scanner.nextLine();
                        out.writeObject(String.valueOf(choice));

                        out.writeObject((String.valueOf(indexOfFile)));

                        out.flush();

                        int bytesRead = 0;
                        int chunk_size=Integer.parseInt((String) in.readObject());
                        System.out.println("chunk is "+chunk_size);
                        byte[] buffer = new byte[chunk_size];
                        String basePath = new File("").getAbsolutePath();
                        String path = basePath.concat("/downloaded/"+userName+"/"+filename);
                        path=uploadPath+filename;
                        FileOutputStream fileOutputStream = null;
                        try {
                            fileOutputStream = new FileOutputStream(path);
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        }
                        try{
                            long size = 0;
                            while((bytesRead=in.read(buffer, 0, buffer.length))!=-1) {
                                fileOutputStream.write(buffer, 0, bytesRead);
                                size += bytesRead;
                            }
                        }catch (IOException e)
                        {
                            e.printStackTrace();
                        }
                        fileOutputStream.close();
                        String lol=(String)in.readObject();
                        System.out.println(lol);
                        out.flush();
                        System.out.println("File downloaded successfully");
                    }
                    else
                    {
                        out.writeObject(String.valueOf(choice));
                    }

                }
                else if (option.equalsIgnoreCase("1"))
                {
                    out.writeObject(option);
                    int totalUsers = Integer.parseInt((String) in.readObject());
                    System.out.println("There are "+totalUsers+" users in the system");
                    System.out.println("The list is shown below ----------------------------------");
                    System.out.println("");
                    String now="No.,Active_status, Username";
                    String[] res = now.split("[,]", 0);
                    for(String myStr: res) {
                        String result = String.format("%" + 15 + "s", myStr);
                        System.out.print(result);
                    }
                    System.out.println("");
                    for(int i=0;i<totalUsers;i++)
                    {
                        now = (String)in.readObject();
                        res = now.split("[,]", 0);
                        for(String myStr: res) {
                            String result
                                    = String.format("%" + 15 + "s", myStr);
                            System.out.print(result);
                        }
                        System.out.println("");
                    }
                    System.out.println("");
                }
                else if (option.equalsIgnoreCase("5"))
                {
                    out.writeObject(option);
                    System.out.println("Enter the location of the file");
                    String filePath = scanner.nextLine();
                    System.out.println("Enter the status of the file:");
                    System.out.println("1. Private");
                    System.out.println("2. Public");
                    int fileType = Integer.parseInt(scanner.nextLine());
                    long fileSize=0;
                    boolean ok=true;


                    File file = new File(filePath);
                    if( !file.exists() ){
                        System.out.println("File not found");
                        return;
                    }
                    fileSize=file.length();

                    String fileName = file.getName();
                    out.writeObject(fileName);
                    out.writeObject(String.valueOf(fileType));
                    out.writeObject(String.valueOf(fileSize));
                    String result = (String)in.readObject();

                    if(result.equalsIgnoreCase("Possible"))
                    {
                        int chunk_size= Integer.parseInt((String)in.readObject());
                        int fileId=Integer.parseInt((String)in.readObject());
                        System.out.println("Your file id is "+fileId);
                        FileInputStream  fileInputStream = new FileInputStream(file);
                        out.flush();

                        // Here we  break file into chunks
                        byte[] buffer = new byte[chunk_size];

                        int bytes = 0;
                        boolean allok=true;
                        while ((bytes = fileInputStream.read(buffer)) != -1) {
                            out.write(buffer, 0, bytes);
                            out.flush();
                            out.writeObject("ok");
                            // got ashbe
                            socket.setSoTimeout(5000);
                            try{
                                String ack=(String)in.readObject();
                                if(ack.equalsIgnoreCase("got"))
                                {
                                    out.writeObject("foundGot");
                                }
                                else
                                {
                                    out.writeObject("notFoundGot");
                                    allok=false;
                                    break;
                                }
                            }
                            catch (SocketTimeoutException s)
                            {
                                String ack=(String)in.readObject();
                                System.out.println("Timeout hoise reeeeee: "+s);
                                out.writeObject("timeout");
                                socket.setSoTimeout(0);
                                ok=false;
                            }

                            socket.setSoTimeout(0);
                            out.flush();
                            if(ok==false)
                                break;;
                        }
                        out.flush();
                        if(ok==true)
                         System.out.println("File sent to server");
                        else
                            System.out.println("Server took more than 30 seconds so file could not be sent.");
                        fileInputStream.close();
                        out.writeObject("Ended");


                        if(fileType==1 || ok==false)
                        {
                            continue;
                        }


                        System.out.println("Did you want to response to some file request?");
                        System.out.println("1. YES");
                        System.out.println("2. NO");
                        int reqOption=Integer.parseInt(scanner.nextLine());
                        out.writeObject(String.valueOf(reqOption));
                        if(reqOption==1)
                        {
                            int totalReqs = Integer.parseInt((String)in.readObject());
                            for(int i=1;i<=totalReqs;i++)
                            {
                                String msg1 = (String)in.readObject();
                                System.out.println(msg1);
                                String msg2=(String)in.readObject();
                                System.out.println(msg2);
                            }
                            System.out.println("Enter req ID that you wanted to response to:");
                            int reqID=Integer.parseInt(scanner.nextLine());
                            out.writeObject(String.valueOf(reqID));
                            System.out.println("The corresponding user has been notified about your uploaded file, thanks!");
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        System.out.println("Not possible due to excessive file size");
                    }
                }
                else if (option.equalsIgnoreCase("6"))
                {
                    out.writeObject(option);
                    System.out.println("Enter description of what do you want?");
                    msg=scanner.nextLine();
                    out.writeObject(msg);
                    int id=Integer.parseInt((String)in.readObject());
                    System.out.println("Your request Id is "+id);
                }
                else if (option.equalsIgnoreCase("4"))
                {
                    out.writeObject(option);
                    int totalcount=(Integer.parseInt((String)in.readObject()));
                    System.out.println("There are "+totalcount+" unread messages for you.");
                    System.out.println("");
                    for(int i=1;i<=totalcount;i++) {
                        System.out.println("-------------------------------------------------------------------");
                        String sender = (String) in.readObject();
                        msg = (String) in.readObject();
                        System.out.println("Message Number - "+i+":  ");
                        System.out.println(msg);
                        System.out.println("");
                    }
                }
                else
                {
                    System.out.println("Please enter correct option");
                }
            }
        }
        else
        {
            System.out.println(msg);
        }
    }
}

