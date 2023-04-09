import java.util.ArrayList;
import java.util.List;

public class Professor extends Subject {
    private String name;

    private StringBuffer buf;
    public Professor(String name) {
        this.name = name;
        buf = new StringBuffer();
    }
    public void talk(String msg) {
        buf.append(msg);
        notifyObservers();

    }
    public String getContent() {
        return buf.toString();
    }
}
