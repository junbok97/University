public class LampFactory {
    public static DirectionLamp createLamp(VendorId id) {
        DirectionLamp lamp = null;
        switch (id) {
            case LG: lamp = new LgLamp(); break;
            case HYUNDAI:lamp = new HyundaiLamp(); break;
        }
        return lamp;
    }
}
