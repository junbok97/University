public class Main {
    public static void main(String[] args) {
        Bird bird = new Bird();
        Command command = new BirdFlyCmd(bird);
        Controller controller = new Controller();
        controller.setCommand(command);
        controller.exec();
    }
}
