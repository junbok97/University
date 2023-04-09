public class HyundaiElevatorFactory extends ElevatorAbstractFactory {
    @Override
    public Motor createMotor() {
        return new HyundaiMotor();
    }

    @Override
    public Door createDoor() {
        return new HyundaiDoor();
    }

    @Override
    public DirectionLamp createLamp() {
        return new HyundaiLamp();
    }

    @Override
    public Elevator createElevator() {
        return new HyundaiElevator();
    }
}
