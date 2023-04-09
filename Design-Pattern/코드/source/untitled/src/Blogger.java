import java.util.ArrayList;
import java.util.List;

public class Blogger {
    private List<Subscriber> subscribers = new ArrayList<>() ;
    public void join(Subscriber subscriber) { subscribers.add(subscriber) ; }
    public void broadcast() {
        for ( Subscriber s : subscribers ) s.update() ;
    }
}