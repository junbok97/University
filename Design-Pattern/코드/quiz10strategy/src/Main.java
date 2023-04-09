public class Main {
    public static void main(String[] args) {
        Monkey monkey = new Monkey(new Jumping());
        monkey.exec();
        monkey.setMovingStratgey(new Walking());
        monkey.exec();
    }
}
