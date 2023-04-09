import java.awt.*;

import static javax.swing.JFrame.EXIT_ON_CLOSE;

public class MazeCreator{
    public static Maze createMaze(MazeFactory factory) {
        Maze maze = factory.makeMaze();

        Room room1 = factory.makeRoom(1);
        Room room2 = factory.makeRoom(2);
        Room room3 = factory.makeRoom(3);
        Room room4 = factory.makeRoom(4);
        Room room5 = factory.makeRoom(5);
        Room room6 = factory.makeRoom(6);

        Door door1 = factory.makeDoor(room1, room3);
        Door door2 = factory.makeDoor(room2, room3);
        Door door3 = factory.makeDoor(room3, room4);
        Door door4 = factory.makeDoor(room2, room5);
        Door door5 = factory.makeDoor(room5, room6);
        Door door6 = factory.makeDoor(room3, room6);

        room1.setSide(Direction.NORTH, new Wall());
        room1.setSide(Direction.EAST, door1);
        room1.setSide(Direction.SOUTH, new Wall());
        room1.setSide(Direction.WEST, new Wall());

        room2.setSide(Direction.NORTH, new Wall());
        room2.setSide(Direction.EAST, door4);
        room2.setSide(Direction.SOUTH, door2);
        room2.setSide(Direction.WEST, new Wall());

        room3.setSide(Direction.NORTH, door2);
        room3.setSide(Direction.EAST, door6);
        room3.setSide(Direction.SOUTH, door3);
        room3.setSide(Direction.WEST, door1);

        room4.setSide(Direction.NORTH, door3);
        room4.setSide(Direction.EAST, new Wall());
        room4.setSide(Direction.SOUTH, new Wall());
        room4.setSide(Direction.WEST, new Wall());

        room5.setSide(Direction.NORTH, new Wall());
        room5.setSide(Direction.EAST, new Wall());
        room5.setSide(Direction.SOUTH, door5);
        room5.setSide(Direction.WEST, door4);

        room6.setSide(Direction.NORTH, door5);
        room6.setSide(Direction.EAST, new Wall());
        room6.setSide(Direction.SOUTH, new Wall());
        room6.setSide(Direction.WEST, door6);

        maze.addRoom(room1);
        maze.addRoom(room2);
        maze.addRoom(room3);
        maze.addRoom(room4);
        maze.addRoom(room5);
        maze.addRoom(room6);

        maze.setCurrentRoom(1);

        return maze;
    }




    public static void main(String[] args) {
        MazeFactory factory = new MyMazeFactory();
        Maze maze = createMaze(factory);
        MazeController controller = MazeController.getInstance();
        controller.setMaze(maze);
        controller.setVisible(true);
        controller.setSize(new Dimension(300, 200));
        controller.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}
