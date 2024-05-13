package HttpServer;



import com.sun.net.httpserver.HttpServer;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static int PORT = 5022;

    public static void main(String[] args) {
        File baseDir = new File("");
        String logPath = baseDir.getAbsolutePath();
        System.out.println(logPath);


        String uploadPath = baseDir.getAbsolutePath().concat("/uploaded");
        File uploadDirectory = new File(uploadPath);
        if( !uploadDirectory.exists() ) uploadDirectory.mkdir();
        try {
            ServerSocket server = new ServerSocket(PORT, 100);
            System.out.println("Server started");
            while (true) {
                Socket connection = server.accept();
                System.out.println("New Client connected!");
                ServerThread serverThread= new ServerThread(connection);
                serverThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
