public class Main {
    public static void main(String[] args) {
        Report r = new Sharp(new Star(new BasicReport("Design Pattern")));
        r.displayContent();

        System.out.println("\n-------------\n");

        Report r1 = new Star(new Sharp(new BasicReport("Design Pattern")));
        r1.displayContent();
    }
}
