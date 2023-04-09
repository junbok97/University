public class Gold {
    private int weight;

    public Gold(int weight) {
        this.weight = weight;
    }

    public int price() {
        return this.weight/100*200;
    }
}
