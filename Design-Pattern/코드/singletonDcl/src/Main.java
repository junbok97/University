public class Main {
    public static void main(String[] args) {
        Incrementor u = new Incrementor(Counter.getInstance());
        Decrementor d = new Decrementor(Counter.getInstance());
        u.start();
        d.start();

        try {
            u.join();
            d.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println(Counter.getInstance().getCount());

    }
}
