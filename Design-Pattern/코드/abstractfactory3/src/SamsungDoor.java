public class SamsungDoor extends Door {
    @Override
    protected void doClose() {
        System.out.println("close samsung door");
    }

    @Override
    protected void doOpen() {
        System.out.println("open samsung door");
    }
}
