class Sharp extends ReportAdded {
    public Sharp(Report report) {
        super(report);
    }

    public void displayContent() {
        displayStar();
        super.displayContent();
        displayStar();
    }

    private void displayStar() {
        System.out.println("################");
    }
}
