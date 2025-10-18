using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;

namespace KrkrReinsertBres;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window {
	public MainWindow() {
		InitializeComponent();
	}

	private void OpenFileBtn_Click(object sender, RoutedEventArgs e) {
		OpenFileDialog dialog = new OpenFileDialog();
		if (dialog.ShowDialog() == true) {
			ifTextBox.Text = dialog.FileName;
		}
	}

	private void SaveFileBtn_Click(object sender, RoutedEventArgs e) {
		SaveFileDialog dialog = new SaveFileDialog();
		if (dialog.ShowDialog() == true) {
			ofTextBox.Text = dialog.FileName;
		}
	}

	private void RunBtn_Click(object sender, RoutedEventArgs e) {
		if (!File.Exists(ifTextBox.Text)) {
			MessageBox.Show("Input file does not exist.");
			return;
		}
		byte[] state;
		try {
			state = fbTextBox
				.Text
				.Split(',')
				.Select(s => {
					if (!byte.TryParse(s, out byte value))
						throw new FormatException();
					return value;
				})
				.ToArray();
			if (state.Length != 64) throw new FormatException();
		}
		catch {
			MessageBox.Show("Wrong block format.");
			return;
		}
		byte[] data = File.ReadAllBytes(ifTextBox.Text);
		uint[] s = new uint[16];
		Buffer.BlockCopy(state, 0, s, 0, 64);
		for (int l = 0; l < 16; l++) {
			s[l] = ~s[l];
		}
		if (bseRadio.IsChecked == true) {
			data = Crypt.ZlibEnc(data);
		}
		data = Crypt.Chacha20(data, s);
		if (bsdRadio.IsChecked == true) {
			data = Crypt.ZlibDec(data);
		}
		File.WriteAllBytes(ofTextBox.Text, data);
		MessageBox.Show("Data written to file.");
	}
}