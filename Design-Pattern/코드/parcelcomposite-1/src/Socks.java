public class Socks {
    private int weight;

    public Socks(int weight) {
        this.weight = weight;
    }

    public int price() {
        return this.weight/100*200;
    }
}
