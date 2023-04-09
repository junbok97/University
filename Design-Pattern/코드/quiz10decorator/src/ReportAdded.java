class ReportAdded extends Report {
    private Report report;

    public ReportAdded(Report report) {
        this.report = report;
    }

    @Override
    public void displayContent() {
        report.displayContent();
    }
}
