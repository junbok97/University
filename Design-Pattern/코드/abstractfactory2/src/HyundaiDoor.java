public class HyundaiDoor extends Door {
    @Override
    protected void doClose() {
        System.out.println("Close Hyundai Door");
    }

    @Override
    public void doOpen() {
        System.out.println("Open Hyundai Door");
    }
}
