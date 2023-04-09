public class SamsungElevatorFactory extends ElevatorAbstractFactory {
    @Override
    public Motor createMotor() {
        return new SamsungMotor();
    }

    @Override
    public Door createDoor() {
        return new SamsungDoor();
    }

    @Override
    public DirectionLamp createLamp() {
        return new SamsungLamp();
    }

    @Override
    public Elevator createElevator() {
        return new SamsungElevator();
    }
}
