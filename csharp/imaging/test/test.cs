using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using System.Diagnostics;

using AVG.Automation.ZEN;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {

            Stopwatch stopwatch = new Stopwatch();

            Bitmap a = new Bitmap("./images/A.png");
            Bitmap b = new Bitmap("./images/B.png");
            Bitmap c = new Bitmap("./images/C.png");
            Bitmap d = new Bitmap("./images/D.png");
            Bitmap e = new Bitmap("./images/D.png");


            // Test cases enabling
            bool testBitmapEditor = true;
            bool testBitmapDifference = true;
            bool testBitmapComparator = true;
            bool testBitmapBatchComparator = true;
            bool testIntegration = true;


            // CLASS BitmapEditor
            if (testBitmapEditor)
            {
                // BitmapEditor.SetPixel()
                // Bitmap <-> BitmapEditor implicit conversions
                {
                    Bitmap bitmap = new Bitmap(4, 4, PixelFormat.Format24bppRgb);
                    BitmapEditor bitmapEditor = bitmap;
                    bitmapEditor.SetPixel(0, 0, Color.Cyan);
                    bitmapEditor.SetPixel(1, 1, Color.Cyan);
                    bitmapEditor.SetPixel(2, 2, Color.Cyan);
                    bitmap = bitmapEditor;
                    bitmap.Save("./bitmapEditorConversions1.png");
                }


                // BitmapEditor.SetPixel()
                // Bitmap <-> BitmapEditor explicit conversions
                {
                    BitmapEditor bitmapEditor = new BitmapEditor(new Bitmap(a));
                    bitmapEditor.SetPixel(0, 0, Color.Cyan);
                    bitmapEditor.SetPixel(1, 1, Color.Cyan);
                    bitmapEditor.SetPixel(2, 2, Color.Cyan);
                    bitmapEditor.SetPixel(3, 3, Color.Cyan);
                    bitmapEditor.SetPixel(4, 4, Color.Cyan);
                    bitmapEditor.SetPixel(5, 5, Color.Cyan);
                    bitmapEditor.SetPixel(6, 6, Color.Cyan);
                    bitmapEditor.SetPixel(100, 100, Color.Cyan);
                    bitmapEditor.SetPixel(101, 100, Color.Cyan);
                    bitmapEditor.SetPixel(102, 100, Color.Cyan);
                    bitmapEditor.SetPixel(103, 100, Color.Cyan);
                    ((Bitmap)(bitmapEditor)).Save("./bitmapEditorConversions2.png");
                }
            }


            // CLASS BitmapDifference TEST
            if (testBitmapDifference)
            {
                {
                    // BitmapDifference from Bitmap
                    // BitmapDifference.SetPixel() - marking pixels as different
                    // BitmapDifference.GetBitmapDifferenceImage

                    BitmapDifference bitmapDifferenceA = new BitmapDifference(a);
                    bitmapDifferenceA.SetPixel(100, 100, true);
                    bitmapDifferenceA.SetPixel(100, 101, true);
                    bitmapDifferenceA.SetPixel(100, 102, true);
                    bitmapDifferenceA.SetPixel(100, 103, true);
                    bitmapDifferenceA.GetBitmapDifferenceImage().Save("./bitmapDifferenceImage.png");


                    // BitmapDifference from Bitmap
                    // BitmapDifference.AddMask()
                    // BitmapDifference.GetBitmapMaskedImage()

                    BitmapDifference bitmapDifferenceB = new BitmapDifference(a);
                    bitmapDifferenceB.AddMask(bitmapDifferenceA.GetBitmapDifferenceImage());
                    bitmapDifferenceB.GetBitmapMaskedImage().Save("./bitmapDifferenceMaskedImage.png");
                }
            }


            // CLASS BitmapComparator TEST
            if (testBitmapComparator)
            {
                BitmapComparator bitmapComparator = new BitmapComparator(a, b);
                bitmapComparator.GetBitmapDifferenceImage().Save("./bitmapComparatorDiffImg.png");
            }


            // CLASS BitmapBatchComparator TEST
            if (testBitmapBatchComparator)
            {
                BitmapBatch bitmapBatch = new BitmapBatch();
                bitmapBatch.Add(a);
                bitmapBatch.Add(b);
                bitmapBatch.Add(c);
                bitmapBatch.Add(d);
                bitmapBatch.Add(e);
                BitmapBatchComparator bitmapBatchComparator = new BitmapBatchComparator(bitmapBatch);
                bitmapBatchComparator.GetBitmapDifferenceImage().Save("./bitmapBatchComparatorDiffImg.png");
            }


            // CLASS integration TEST
            if (testIntegration)
            {
                stopwatch.Restart();

                BitmapBatch bbb = new BitmapBatch();

                Console.WriteLine("A: " + stopwatch.ElapsedMilliseconds);

                bbb = Screener.GetScreenshotBatch(3000, 250);

                Console.WriteLine("B: " + stopwatch.ElapsedMilliseconds);

                BitmapBatchComparator bbcb = new BitmapBatchComparator(bbb);

                Console.WriteLine("C: " + stopwatch.ElapsedMilliseconds);

                bbcb.GetBitmapDifferenceImage().Save("./bitmapBatchDifference.png");

                Console.WriteLine("D: " + stopwatch.ElapsedMilliseconds);

                BitmapComparator ndnjjcd = new BitmapComparator(a, b);

                Console.WriteLine("E: " + stopwatch.ElapsedMilliseconds);

            }

            Console.WriteLine("Done.");
            Console.ReadKey();
        }
    }
}
