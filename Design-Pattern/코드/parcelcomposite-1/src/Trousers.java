public class Trousers {
    private int weight;

    public Trousers(int weight) {
        this.weight = weight;
    }

    public int price() {
        return this.weight/100*200;
    }
}
