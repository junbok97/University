import java.util.ArrayList;
import java.util.List;

public class Box {
    private List<Trousers> trousers = new ArrayList<>();
    private List<Socks> socks = new ArrayList<>();
    private List<Gold> golds = new ArrayList<>();
    private List<Box> boxes = new ArrayList<>();

    public int price() {
        int tPrice = 0;
        int sPrice = 0;
        int gPrice = 0;
        int bPrice = 0;

        for (Trousers t : trousers) {
            tPrice += t.price();
        }
        for (Socks s : socks) {
            sPrice += s.price();
        }
        for (Gold g : golds) {
            gPrice += g.price();
        }
        for (Box b : boxes) {
            bPrice += b.price();
        }

        return tPrice + sPrice + gPrice + bPrice;
    }
    public void addSocks(Socks s) { socks.add(s); }
    public void addTrousers(Trousers t) { trousers.add(t); }
    public void addGolds(Gold g) { golds.add(g); }
    public void addBox(Box b) { boxes.add(b); }

}
