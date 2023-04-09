public class MotorFactory {
    public static Motor createMotor(VendorId id) {
        Motor motor = null;
        switch (id) {
            case LG: motor = new LgMotor(); break;
            case HYUNDAI:motor = new HyundaiMotor(); break;
        }
        return motor;
    }
}
