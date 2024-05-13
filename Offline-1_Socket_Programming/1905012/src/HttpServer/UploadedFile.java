package HttpServer;

import java.time.LocalDate;
import java.util.Date;

class UploadedFile {
    public int size;
    public int fileId;
    public String owner;
    public String status;
    public String URL;
    public LocalDate date;
    public String fileName;

    public UploadedFile(String fileName,int fileId, int size, String owner, String status, String URL, LocalDate date) {
        this.fileName=fileName;
        this.fileId=fileId;
        this.size=size;
        this.owner=owner;
        this.status=status;
        this.URL=URL;
        this.date = date;
    }
}
