public class Recorder implements Observer {
    private Professor professor;

    public void setProfessor(Professor professor) {
        this.professor = professor;
    }

    public void update() {
        String content = professor.getContent();
        record(content);
    }

    private void record(String content) {
        System.out.print("Recorder: "+content);
    }
}
