public class ElevatorFactory {
    public static Elevator createElevator(VendorId id) {
        Elevator elevator = null;
        switch (id) {
            case LG: elevator = new LgElevator(); break;
            case HYUNDAI:elevator = new HyundaiElevator(); break;
        }
        return elevator;
    }
}
