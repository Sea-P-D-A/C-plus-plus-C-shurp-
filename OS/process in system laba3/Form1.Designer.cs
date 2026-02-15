namespace OS_laba3
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;
        private Button scanButton;
        private TextBox minIdTextBox;
        private TextBox maxIdTextBox;
        private DataGridView resultsDataGridView;
        private Label minIdLabel;
        private Label maxIdLabel;
        private ProgressBar progressBar;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.scanButton = new Button();
            this.minIdTextBox = new TextBox();
            this.maxIdTextBox = new TextBox();
            this.resultsDataGridView = new DataGridView();
            this.minIdLabel = new Label();
            this.maxIdLabel = new Label();
            this.progressBar = new ProgressBar();

            ((System.ComponentModel.ISupportInitialize)(this.resultsDataGridView)).BeginInit();
            this.SuspendLayout();

            // minIdLabel
            this.minIdLabel.Location = new Point(20, 20);
            this.minIdLabel.Size = new Size(100, 20);
            this.minIdLabel.Text = "Минимальный ID:";

            // minIdTextBox
            this.minIdTextBox.Location = new Point(120, 20);
            this.minIdTextBox.Size = new Size(100, 20);
            this.minIdTextBox.Text = "0";

            // maxIdLabel
            this.maxIdLabel.Location = new Point(240, 20);
            this.maxIdLabel.Size = new Size(100, 20);
            this.maxIdLabel.Text = "Максимальный ID:";

            // maxIdTextBox
            this.maxIdTextBox.Location = new Point(340, 20);
            this.maxIdTextBox.Size = new Size(100, 20);
            this.maxIdTextBox.Text = "10000";

            // scanButton
            this.scanButton.Location = new Point(460, 18);
            this.scanButton.Size = new Size(100, 25);
            this.scanButton.Text = "Сканировать";
            this.scanButton.Click += new EventHandler(this.scanButton_Click);

            // progressBar
            this.progressBar.Location = new Point(20, 60);
            this.progressBar.Size = new Size(540, 20);
            this.progressBar.Visible = false;

            // resultsDataGridView
            this.resultsDataGridView.Location = new Point(20, 100);
            this.resultsDataGridView.Size = new Size(540, 300);
            this.resultsDataGridView.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            this.resultsDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            this.resultsDataGridView.ReadOnly = true;
            this.resultsDataGridView.ScrollBars = ScrollBars.Vertical;

            // Form
            this.ClientSize = new Size(584, 421);
            this.Controls.Add(this.scanButton);
            this.Controls.Add(this.minIdTextBox);
            this.Controls.Add(this.maxIdTextBox);
            this.Controls.Add(this.resultsDataGridView);
            this.Controls.Add(this.minIdLabel);
            this.Controls.Add(this.maxIdLabel);
            this.Controls.Add(this.progressBar);
            this.Text = "Сканер модулей процессов";
            this.MinimumSize = new Size(600, 400);

            ((System.ComponentModel.ISupportInitialize)(this.resultsDataGridView)).EndInit();
            this.ResumeLayout(false);
        }
    }
}