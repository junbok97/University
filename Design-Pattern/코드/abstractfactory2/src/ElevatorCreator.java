public class ElevatorCreator {
    public static Elevator assembleElevator(VendorId id) {
        Elevator elevator = ElevatorFactory.createElevator(id);
        Motor motor = MotorFactory.createMotor(id);
        elevator.setMotor(motor);
        Door door = DoorFactory.createDoor(id);
        elevator.setDoor(door);
        motor.setDoor(door);
        DirectionLamp lamp = LampFactory.createLamp(id);
        elevator.setLamp(lamp);
        return elevator;
    }

    public static void main(String[] args) {

        Elevator elevator = assembleElevator(VendorId.HYUNDAI);
        elevator.move(Direction.UP);
    }
}
