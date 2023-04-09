public class HyundaiMotor extends Motor {
    @Override
   protected void moveMotor(Direction direction) {
        System.out.println("Hyundai motor is Moving " + direction);
    }
}
