using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace FileManager
{
    public partial class Form1 : Form
    {
        string LastFilename { get; set; } = "";
        string LastZipname { get; set; } = "";
        bool IsSaved { get; set; } = true;
        int LMaxSize { get; } = 100;
        LinkedList<Data> back;
        LinkedList<Data> front;

        public Form1(string text)
        {
            InitializeComponent();

            textBox1.Text = text;
            Text = "untitled.txt";

            back = new LinkedList<Data>();
            front = new LinkedList<Data>();
            back.AddLast(new Data(text, GetPos()));
        }

        private void SaveTxt(string filename)
        {
            File.WriteAllText(filename, textBox1.Text);
        }
        private void SaveDat(string filename)
        {
            BinaryWriter writer = new BinaryWriter(File.Open(filename, FileMode.OpenOrCreate));
            writer.Write(textBox1.Text);
            writer.Close();
        }
        private void OpenTxt(string filename)
        {
            textBox1.Text = File.ReadAllText(filename);
        }
        private void OpenDat(string filename)
        {
            BinaryReader reader = new BinaryReader(File.Open(filename, FileMode.Open));
            textBox1.Text = reader.ReadString();
            reader.Close();
        }
        private void GlobalOpen()
        {
            if (SaveIsChecked())
            {
                OpenFileDialog dialog = new OpenFileDialog();
                dialog.Filter = "Text files(*.txt)|*.txt|Binary files(*.dat)|*.dat";
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    Text = Path.GetFileName(dialog.FileName);
                    LastFilename = dialog.FileName;
                    LastZipname = "";
                    if (Path.GetExtension(LastFilename) == ".txt")
                        OpenTxt(LastFilename);
                    if (Path.GetExtension(LastFilename) == ".dat")
                        OpenDat(LastFilename);
                    IsSaved = true;
                    back.Clear();
                    front.Clear();
                    back.AddLast(new Data(textBox1.Text, GetPos()));
                }
            }
        }
        private void GlobalSave()
        {
            if (LastFilename != "")
            {
                if (Path.GetExtension(LastFilename) == ".txt")
                    SaveTxt(LastFilename);
                if (Path.GetExtension(LastFilename) == ".dat")
                    SaveDat(LastFilename);
                IsSaved = true;
            }
            else
            {
                GlobalSaveAs();
            }
        }
        private void GlobalSaveAs()
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "Text files(*.txt)|*.txt|Binary files(*.dat)|*.dat";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                Text = Path.GetFileName(dialog.FileName);
                LastFilename = dialog.FileName;
                if (dialog.FilterIndex == 1)
                    SaveTxt(LastFilename);
                else
                {
                    SaveDat(LastFilename);
                }
                IsSaved = true;
            }
        }
        private void GlobalNew()
        {
            if (SaveIsChecked())
            {
                Text = "untitled.txt";
                textBox1.Text = "";
                LastFilename = "";
                LastZipname = "";
                IsSaved = true;
                back.Clear();
                front.Clear();
                back.AddLast(new Data(textBox1.Text, GetPos()));
            }
        }
        private void Zip()
        {
            using (FileStream zipToOpen = new FileStream(LastZipname, FileMode.Open))
            {
                using (ZipArchive archive = new ZipArchive(zipToOpen, ZipArchiveMode.Update))
                {
                    ZipArchiveEntry readmeEntry = archive.CreateEntry(Text);
                    using (StreamWriter writer = new StreamWriter(readmeEntry.Open()))
                    {
                        writer.Write(textBox1.Text);
                    }
                }
            }
        }
        private void GlobalZip()
        {
            if (LastZipname != "")
            {
                Zip();
            }
            else
            {
                SaveFileDialog dialog = new SaveFileDialog();
                dialog.Filter = "Archive files(*.zip)|*.zip";
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    LastZipname = dialog.FileName;
                    using (FileStream zipFile = File.Open(LastZipname, FileMode.Create)) { } ;
                    Zip();
                }
            }
        }
        private void GlobalUnzip()
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Archive files(*.zip)|*.zip";
            if(dialog.ShowDialog() == DialogResult.OK)
            {
                LastZipname = dialog.FileName;
                using (ZipArchive archive = ZipFile.Open(LastZipname, ZipArchiveMode.Update))
                {
                    int counter = 0;
                    foreach(ZipArchiveEntry entry in archive.Entries)
                    {
                        if(Path.GetExtension(entry.FullName) == ".txt" || Path.GetExtension(entry.FullName) == ".dat")
                            using (StreamReader reader = new StreamReader(entry.Open()))
                            {
                                if (counter == 0)
                                {
                                    GlobalNew();
                                    textBox1.Text = reader.ReadToEnd();
                                }
                                else
                                {
                                    Thread thread = new Thread(new ParameterizedThreadStart(SummonForm));//
                                    thread.SetApartmentState(ApartmentState.STA);
                                    thread.Start(reader.ReadToEnd());
                                }
                                counter++;
                                entry.LastWriteTime = DateTimeOffset.UtcNow.LocalDateTime;
                            }                      
                    }
                    if (counter == 0)
                    {
                        MessageBox.Show("Txt or dat files are not found", "Notepad", MessageBoxButtons.OK);
                    }
                }
            }
        }
        private void SummonForm(object text)
        {
            Form1 form = new Form1(text.ToString());
            Application.Run(form);
        }
        private bool SaveIsChecked()
        {
            if(!IsSaved)
            {
                DialogResult res = MessageBox.Show("Do you want to save current file?", "Notepad", MessageBoxButtons.YesNoCancel);
                if(res == DialogResult.Yes)
                {
                    GlobalSave();
                    if (IsSaved)
                        return true;
                    else
                        return false;
                }
                else if(res == DialogResult.No)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalSave();
        }
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalSaveAs();
        }
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalOpen();
        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            IsSaved = false;

            var temp = new Data(textBox1.Text, GetPos());
            if (temp != back.Last.Value)
            {
                back.AddLast(temp);
                if (back.Count == LMaxSize + 1)
                    back.RemoveFirst();
            }
        }
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalNew();
        }
        private void archiveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalZip();
        }
        private void unzipToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlobalUnzip();
        }

        private void toolStripMenuItem3_Click(object sender, EventArgs e)
        {
            if(back.Count > 1)
            {
                front.AddFirst(back.Last.Value);
                back.RemoveLast();
                textBox1.Text = back.Last.Value.Text;
                moveAt(back.Last.Value.CarP);
            }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if(front.Count != 0)
            {
                back.AddLast(front.First.Value);
                front.RemoveFirst();
                textBox1.Text = back.Last.Value.Text;
                moveAt(back.Last.Value.CarP);
            }
        }

        private void moveAt(int charsFromStart)
        {
            textBox1.Select(charsFromStart, 0);
            textBox1.Focus();
        }

        private int GetPos()
        {
            int res = 0;
            foreach(string line in textBox1.Lines)
            {
                res += line.Length + 2;
            }
            return res;
        }
    }

    public class Data
    {
        public Data(string text, int carP) { Text = text; CarP = carP; }

        public string Text { get; set; }
        public int CarP { get; set; }

        public static bool operator ==(Data a, Data b)
        {
            return a.Text == b.Text && a.CarP == b.CarP;
        }

        public static bool operator !=(Data a, Data b)
        {
            return !(a==b);
        }
    }
}
