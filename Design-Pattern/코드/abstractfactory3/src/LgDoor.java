public class LgDoor extends Door {
    @Override
    protected void doClose() {
        System.out.println("Close Lg Door");
    }

    @Override
    protected void doOpen() {
        System.out.println("Open Lg Door");
    }
}
