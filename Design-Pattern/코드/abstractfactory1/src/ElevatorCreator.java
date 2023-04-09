public class ElevatorCreator {
    public static Elevator assembleElevator() {
        Elevator elevator = new LgElevator();
        Motor motor = new LgMotor();
        elevator.setMotor(motor);
        Door door = new LgDoor();
        elevator.setDoor(door);
        motor.setDoor(door);
        DirectionLamp lamp = new LgLamp();
        elevator.setLamp(lamp);
        return elevator;
    }

    public static void main(String[] args) {

        Elevator elevator = assembleElevator();
        elevator.move(Direction.UP);
    }
}
