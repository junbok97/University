public class DoorFactory {
    public static Door createDoor(VendorId id) {
        Door door = null;
        switch (id) {
            case LG: door = new LgDoor(); break;
            case HYUNDAI: door = new HyundaiDoor(); break;
        }
        return door;
    }
}
