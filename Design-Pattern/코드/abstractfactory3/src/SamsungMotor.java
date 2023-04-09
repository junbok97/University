public class SamsungMotor extends Motor {
    @Override
    protected void moveMotor(Direction direction) {
        System.out.println("Samsung Motor is moving " + direction);
    }
}
