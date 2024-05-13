package HttpServer;

class Request {
    int requestId;
    String description;
    String senderName;
    String fulfilledBy;

    public Request(int requestId, String description, String senderName, String fulfilledBy) {
        this.requestId = requestId;
        this.description = description;
        this.senderName = senderName;
        this.fulfilledBy = fulfilledBy;
    }
}
