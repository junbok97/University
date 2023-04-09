public class Main {
    public static void main(String[] args) {
        Box box1 = new Box();
        Socks s1 = new Socks(200);
        Socks s2 = new Socks(300);
        Trousers t1 = new Trousers(600);
        box1.addSocks(s1);
        box1.addSocks(s2);
        box1.addTrousers(t1);
        System.out.println(box1.price());

        Box box2 = new Box();
        Gold g1 = new Gold(800);
        box2.addBox(box1);
        box2.addGolds(g1);
        System.out.println(box2.price());

    }
}
