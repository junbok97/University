public class Main {
    public static void main(String[] args) {
//        Display d1 = new Display(new StringDisplayImpl("Hello, Korea!"));
        CountDisplay d2 = new CountDisplay(new StringDisplayImpl("Hello, Universe!"));

//        d1.display();
//        d2.display();

        d2.multiDisplay(5);
    }
}
