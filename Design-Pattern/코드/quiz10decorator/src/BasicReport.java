class BasicReport extends Report {
    private String msg;

    public BasicReport(String msg) {
        this.msg = msg;
    }
    @Override
    public void displayContent() {
        System.out.println(msg);
    }
}
