using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace AVG.Automation.ZEN
{
    /// <summary>
    /// Compares two Bitmap objects.
    /// </summary>
    public class BitmapComparator
    {
        public enum Result { Identical, DifferentColor, DifferentSize }

        private Result              bitmapComparatorResult;
        private BitmapDifference    bitmapComparatorDifference;
        private BitmapEditor        bitmapComparatorBitmapA;
        private BitmapEditor        bitmapComparatorBitmapB;

        /// <summary>
        /// Constructor initializes and compares pixel-by-pixel two Bitmap objects
        /// </summary>
        /// <param name="a">First Bitmap object (will serve as base to display differences on)</param>
        /// <param name="b">Second Bitmap object which will be compared to the first</param>
        public BitmapComparator(BitmapEditor a, BitmapEditor b)
        {
            bitmapComparatorBitmapA = a;
            bitmapComparatorBitmapB = b;

            bitmapComparatorDifference = new BitmapDifference(bitmapComparatorBitmapA);

            Compare();
        }

        /// <summary>
        /// Constructor initializes and compares pixel-by-pixel two Bitmap objects with predefined BitmapDifference object
        /// </summary>
        /// <param name="a">First Bitmap object (will serve as base to display differences on)</param>
        /// <param name="b">Second Bitmap object which will be compared to the first</param>
        /// <param name="d">Predefined BitmapDifference object (can be used to accumulate differences from multiple comparisons)</param>
        public BitmapComparator(BitmapEditor a, BitmapEditor b, BitmapDifference d)
        {
            bitmapComparatorBitmapA = a;
            bitmapComparatorBitmapB = b;

            bitmapComparatorDifference = d;

            Compare();
        }

        unsafe private void Compare()
        {
            // Compare dimensions of images A and B
            if (bitmapComparatorBitmapA.Width != bitmapComparatorBitmapB.Width || bitmapComparatorBitmapA.Height != bitmapComparatorBitmapB.Height)
            {
                // We don't know how to compare Bitmaps of different sizes
                // Return info that Bitmaps dimensions are different
                bitmapComparatorResult = Result.DifferentSize;
            }
            else
            {
                int width = bitmapComparatorBitmapA.Width;
                int height = bitmapComparatorBitmapA.Height;

                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        if (bitmapComparatorBitmapA.GetPixel(x, y) != bitmapComparatorBitmapB.GetPixel(x, y))
                        {
                            // Set pixel to Cyan in difference image
                            bitmapComparatorDifference.SetPixel(x, y, true);
                        }
                    }
                }

                // Evaluate comparison result
                if (bitmapComparatorDifference.GetBitmapDifferenceMetric() > 0)
                {
                    bitmapComparatorResult = Result.DifferentColor;
                }
                else
                {
                    bitmapComparatorResult = Result.Identical;
                }
            }
        }

        /// <summary>
        /// Adds mask specification marking which pixels should be ignored during comparison. This
        /// method does not influence comparison itself and can be used after it is done. As you add masks
        /// comparison results are calculated against new mask.
        /// </summary>
        /// <param name="mask"></param>
        public void AddBitmapDifferenceMask(Bitmap mask)
        {
            bitmapComparatorDifference.AddMask(mask);
        }

        /// <summary>
        /// Gets the overall result of Bitmap comparison
        /// </summary>
        /// <returns>Returns whether Bitmap object are identical or different</returns>
        public Result GetResult()
        {
            return bitmapComparatorResult;
        }

        /// <summary>
        /// Gets internal BitmapDifference object - the result of Bitmap comparison.
        /// </summary>
        /// <returns></returns>
        public BitmapDifference GetBitmapDifference()
        {
            return bitmapComparatorDifference;
        }

        /// <summary>
        /// Gets the Bitmap with marked different pixels
        /// </summary>
        /// <returns>Returns Bitmap object. If compared Bitmap objects were different in size returns null.</returns>
        public Bitmap GetBitmapDifferenceImage()
        {
            if (bitmapComparatorResult == BitmapComparator.Result.DifferentSize)
            {
                return null;
            }
            else
            {
                return bitmapComparatorDifference.GetBitmapDifferenceImage();
            }
        }

        /// <summary>
        /// Gets the number of different pixels
        /// </summary>
        /// <returns></returns>
        public int GetBitmapDifferenceMetric()
        {
            return bitmapComparatorDifference.GetBitmapDifferenceMetric();
        }
    }
}