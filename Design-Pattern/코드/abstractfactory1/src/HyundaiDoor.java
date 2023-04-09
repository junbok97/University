public class HyundaiDoor extends Door {
    @Override
    protected void doClose() {
        System.out.println("Close Lg Door");
    }

    @Override
    public void doOpen() {
        System.out.println("Open Lg Door");
    }
}
