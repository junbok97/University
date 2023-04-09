public class Blog extends Subject {
    private StringBuffer buf;
    public Blog() {
        buf = new StringBuffer();
    }

    public String getContent() {
        return buf.toString();
    }

    public void changeContent(String content) {
        buf.append(content);
        notifyObservers();
    }
}
