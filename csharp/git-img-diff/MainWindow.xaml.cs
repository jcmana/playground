using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;


namespace gitimgdiff
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private String bitmapAPath;
        private String bitmapBPath;

        private Dictionary<String, Bitmap> bitmap = new Dictionary<String, Bitmap>();
        private Dictionary<String, String> imageTitle = new Dictionary<String, String>()
        {
            { "A", "New image" },
            { "B", "Old image" },
            { "D", "Difference image" }
        };
        private String bitmapCurrent = String.Empty;
        private enum SwitchDirection {Left, Right};

        public MainWindow()
        {
            InitializeComponent();

            // Get application command-line arguments
            string[] applicationArguments = Environment.GetCommandLineArgs();

            // Check number of parameters
            if (applicationArguments.Length < 3)
            {
                Console.WriteLine("Incorrect parameters");
                Console.WriteLine("");
                Console.WriteLine("Usage: git-img-diff.exe <imageA> <imageB>");
                Console.WriteLine("           <imageA> relative or absolute path to first image");
                Console.WriteLine("           <imageB> relative or absolute path to second image");
                
                // Execute application shutdown
                App.Current.Shutdown();

                return;
            }

            // Assign path for each bitmap
            bitmapAPath = applicationArguments[1];
            bitmapBPath = applicationArguments[2];

            // Check imageA path
            if (!File.Exists(bitmapAPath))
            {
                Console.WriteLine(bitmapAPath + ": doesn't exist or is inaccessible");
            }

            // Check imageB path
            if (!File.Exists(bitmapBPath))
            {
                Console.WriteLine(bitmapBPath + ": doesn't exist or is inaccessible");
            }

            // Check if both images are avaible (otherwise we can't continue)
            if (!File.Exists(bitmapAPath) || !File.Exists(bitmapBPath))
            {
                // Execute application shutdown
                App.Current.Shutdown();

                return;
            }

            // Load bitmaps from image files
            bitmap["A"] = new Bitmap(bitmapAPath);
            bitmap["B"] = new Bitmap(bitmapBPath);

            // Initialize Bitmap comparator
            BitmapComparator comparator = new BitmapComparator(bitmap["A"], bitmap["B"]);

            // Announce original image path
            Console.Write(applicationArguments[1] + ": ");

            // Handle comparison result
            switch (comparator.Result)
            {
                case BitmapComparator.ComparisonResult.DifferentSize:
                    // Announce image comparison result
                    Console.WriteLine(" Images differs in size");
                    Console.WriteLine("      Image new: W" + bitmap["A"].Width + " x H" + bitmap["A"].Height);
                    Console.WriteLine("      Image old: W" + bitmap["B"].Width + " x H" + bitmap["B"].Height);

                    // We can't show difference image, so show at least new image
                    Switch("A");

                    break;

                case BitmapComparator.ComparisonResult.DifferentColor:
                    // Announce image comparison result
                    Console.WriteLine("Images differs in " + comparator.PixelDifference + " pixels");

                    // Setup image difference
                    bitmap["D"] = comparator.ImageDifference;

                    // Show difference image
                    Switch("D");

                    break;


                case BitmapComparator.ComparisonResult.Identical:
                    // Announce image comparison result
                    Console.WriteLine("Images are identical");

                    // We can't show difference image, so show at least new image
                    Switch("A");

                    break;
            }
        }

        private BitmapImage BitmapToBitmapImage(Bitmap bitmap)
        {
            // Initialize BitmapImage to hold converted Bitmap
            BitmapImage bitmapImage = new BitmapImage();

            // Load Memory from Bitmap to BitmapImage
            using (MemoryStream memory = new MemoryStream())
            {
                bitmap.Save(memory, ImageFormat.Png);
                memory.Position = 0;
                bitmapImage.BeginInit();
                bitmapImage.StreamSource = memory;
                bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                bitmapImage.EndInit();
            }

            return bitmapImage;
        }

        private void Switch(string index)
        {
            if (bitmap.ContainsKey(index))
            {
                // Set new source for UIElement Image
                image.Source = BitmapToBitmapImage(bitmap[index]);

                // Change currently displayed bitmap index
                bitmapCurrent = index;

                // Change application window title for current image
                if (imageTitle.ContainsKey(index))
                {
                    Title = imageTitle[index];
                }
                else
                {
                    Title = "git-img-diff";
                }
            }
        }
        private void Switch(SwitchDirection direction)
        {
            // Get list of avaible images
            List<String> bitmapKeys = new List<string>(bitmap.Keys);

            int bitmapKeysIndex = bitmapKeys.IndexOf(bitmapCurrent);

            switch (direction)
            {
                case SwitchDirection.Left:
                    if (bitmapKeysIndex > 0)
                    {
                        Switch(bitmapKeys[bitmapKeysIndex - 1]);
                    }
                    else
                    {
                        Switch(bitmapKeys[bitmapKeys.Count - 1]);
                    }
                    break;

                case SwitchDirection.Right:
                    if (bitmapKeysIndex < bitmapKeys.Count - 1)
                    {
                        Switch(bitmapKeys[bitmapKeysIndex + 1]);
                    }
                    else
                    {
                        Switch(bitmapKeys[0]);
                    }
                    break;
            }
        }

        
        /* UI EVENT HANDLERS */

        private void Window_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            switch (e.Key)
            {
                // O - open folder containing new image
                case Key.O:
                    Process.Start("explorer.exe", "/select," + Path.GetFullPath(bitmapAPath));
                    break;

                // LEFT/RIGHT - circle between images
                case Key.Left:
                    Switch(SwitchDirection.Left);
                    break;
                case Key.Right:
                    Switch(SwitchDirection.Right);
                    break;

                // ESCAPE - close application
                case Key.Escape:
                    App.Current.Shutdown();
                    break;
            }
        }
    }
}
