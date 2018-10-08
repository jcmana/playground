using System;
using System.Drawing;

namespace AVG.Automation.ZEN
{
    /// <summary>
    /// Holds information about differences between two Bitmap objects.
    /// </summary>
    public class BitmapDifference
    {
        public enum BitmapDiffereceFlag { Identical, Different }

        private Bitmap                  differenceBaseImage;
        //private BitmapDiffereceFlag[,]  differenceMap;
        private bool[,]                 differenceMap;
        //private BitmapDifferenceMask    differenceMask;
        private bool[,]                 differenceMask;
        private BitmapPixel             differenceColor;
        private BitmapPixel             differenceMaskColor;

        private int width;
        private int height;

        /// <summary>
        /// BitmapDifference constructor initializes to state: no pixels are marked as different.
        /// </summary>
        /// <param name="bitmap">Bitmap serving as base image on which differences will be marked.</param>
        public BitmapDifference(Bitmap bitmap)
        {
            differenceColor = Color.Cyan;
            differenceMaskColor = Color.Cyan;
            differenceBaseImage = new Bitmap(bitmap);
            //differenceMask = new BitmapDifferenceMask(differenceBaseImage.Width, differenceBaseImage.Height);
            differenceMask = new bool[differenceBaseImage.Width, differenceBaseImage.Height];
            //differenceMap = new BitmapDiffereceFlag[differenceBaseImage.Width, differenceBaseImage.Height];
            differenceMap = new bool[differenceBaseImage.Width, differenceBaseImage.Height];

            width = differenceBaseImage.Width;
            height = differenceBaseImage.Height;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // Initial state will say that all pixels are identical
                    differenceMap[x, y] = false;
                }
            }
        }

        /// <summary>
        /// Sets pixel difference flag to value.
        /// </summary>
        /// <param name="x">Coordinate x (column)</param>
        /// <param name="y">Coordinate y (row)</param>
        /// <param name="value">Desired pixel state</param>
        public void SetPixel(int x, int y, bool value)
        {
            // Set pixel difference flag to requested value (Identical or Different)
            differenceMap[x, y] = value;
        }

        /// <summary>
        /// Gets pixel difference flag.
        /// </summary>
        /// <param name="x">Coordinate x (column)</param>
        /// <param name="y">Coordinate y (row)</param>
        /// <returns></returns>
        public bool GetPixel(int x, int y)
        {
            return differenceMap[x, y];
        }

        /// <summary>
        /// Generates masked image where pixels marked as masked are highlighted.
        /// </summary>
        /// <returns></returns>
        public Bitmap GetBitmapMaskedImage()
        {
            // Bitmap difference is base on initial base image
            BitmapEditor maskedImage = new BitmapEditor(differenceBaseImage);

            // Set each Different pixel to specified difference color
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // Pixels are different but not masked
                    if (differenceMask[x, y])
                    {
                        maskedImage.SetPixel(x, y, differenceMaskColor);
                    }
                }
            }

            return maskedImage;
        }

        /// <summary>
        /// Generates difference image where pixels marked as different are highlighted.
        /// </summary>
        /// <returns></returns>
        public Bitmap GetBitmapDifferenceImage()
        {
            // Bitmap difference is base on initial base image
            BitmapEditor differenceImage = new BitmapEditor(differenceBaseImage);

            // Set each Different pixel to specified difference color
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // Pixels are different but not masked
                    if (differenceMap[x, y] == true && differenceMask[x, y] == false)
                    {
                        differenceImage.SetPixel(x, y, differenceColor);
                    }
                }
            }

            return differenceImage;
        }

        /// <summary>
        /// Calculates number of different pixels.
        /// </summary>
        /// <returns></returns>
        public int GetBitmapDifferenceMetric()
        {
            // We start at zero pixels differs
            int differenceMetric = 0;

            // Set each Different pixel to specified difference color
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    if (differenceMap[x, y] == true && differenceMask[x, y] == false)
                    {
                        ++differenceMetric;
                    }
                }
            }

            return differenceMetric;
        }

        /// <summary>
        /// Adds information about which pixels should be ignored when marked as different. Those pixels
        /// won't be counted as different in GetBitmapDifferenceMetric() and GetBitmapDifferenceImage().
        /// </summary>
        /// <param name="mask">Bitmap where pixels of Color.Cyan indicates which pixel should be masked.</param>
        public void AddMask(BitmapEditor mask)
        {
            // Check Bitmap size, we can't continue if sizes doesn't correspond
            if (mask.Width != width || mask.Height != height)
            {
                throw new Exception("Passed Bitmap has different size than initialized mask");
            }

            // Fill mask definition according the passed Bitmap
            // (if pixel on Bitmap is of ignored color - differenceMaskColor - mark it as ignored)
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    if (mask.GetPixel(x, y) == differenceColor)
                    {
                        differenceMask[x, y] = true;
                    }
                }
            }
        }
    }
}
