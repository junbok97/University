import java.util.Optional;

public class Main {
    public static void main(String[] args) {
        TicketManager tm = new TicketManager();
        Optional<Ticket> t1 = tm.issue();
        t1.ifPresent(s->System.out.println(s.getSerialNum()));
        Optional<Ticket> t2 = tm.issue();
        t2.ifPresent(s->System.out.println(s.getSerialNum()));

    }
}
