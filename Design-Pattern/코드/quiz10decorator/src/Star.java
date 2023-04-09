class Star extends ReportAdded {
    public Star(Report report) {
        super(report);
    }

    public void displayContent() {
        displayStar();
        super.displayContent();
        displayStar();
    }

    private void displayStar() {
        System.out.println("****************");
    }
}
