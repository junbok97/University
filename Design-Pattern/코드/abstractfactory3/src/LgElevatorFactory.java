public class LgElevatorFactory extends ElevatorAbstractFactory {
    @Override
    public Motor createMotor() {
        return new LgMotor();
    }

    @Override
    public Door createDoor() {
        return new LgDoor();
    }

    @Override
    public DirectionLamp createLamp() {
        return new LgLamp();
    }

    @Override
    public Elevator createElevator() {
        return new LgElevator();
    }
}
