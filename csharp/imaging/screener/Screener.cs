using System;
using System.Threading;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows;
using System.Windows.Forms;

namespace AVG.Automation.ZEN
{
    public class Screener
    {
        private const PixelFormat pixelFormat = PixelFormat.Format32bppArgb;

        /// <summary>
        /// Captures screenshot of area specified by Rectangle.
        /// </summary>
        /// <param name="sector">Definition of area to capture</param>
        /// <returns></returns>
        public static Bitmap GetScreenshot(Rectangle sector)
        {
            Bitmap screenshot = new Bitmap(sector.Width, sector.Height, pixelFormat);
            Graphics.FromImage(screenshot).CopyFromScreen(sector.Left, sector.Top, 0, 0, sector.Size, CopyPixelOperation.SourceCopy);

            return screenshot;
        }

        /// <summary>
        /// Captures screenshot of area specified by Rect.
        /// </summary>
        /// <param name="sector">Definition of area to capture</param>
        /// <returns></returns>
        public static Bitmap GetScreenshot(Rect sector)
        {
            Rectangle rectangleSector = new Rectangle((int)(Math.Floor(sector.Left)), (int)(Math.Floor(sector.Top)), (int)(Math.Floor(sector.Width)), (int)(Math.Floor(sector.Height)));

            return GetScreenshot(rectangleSector);
        }

        /// <summary>
        /// Captures screenshot of area specified by coordinates and dimensions.
        /// </summary>
        /// <param name="x">Offset from top of the screen</param>
        /// <param name="y">Offset from left of the screen</param>
        /// <param name="width">Width of area</param>
        /// <param name="height">Height of area</param>
        /// <returns></returns>
        public static Bitmap GetScreenshot(int top, int left, int width, int height)
        {
            return GetScreenshot(new Rectangle(top, left, width, height));
        }

        /// <summary>
        /// Captures screenshot of whole primary screen.
        /// </summary>
        /// <returns></returns>
        public static Bitmap GetScreenshot()
        {
            return GetScreenshot(Screen.PrimaryScreen.Bounds);
        }


        /// <summary>
        /// Captures batch of screenshots of area specified by Rectangle.
        /// </summary>
        /// <param name="duration">Duration of taking the screenshots (in milliseconds)</param>
        /// <param name="frequency">Frequency of taking the screenshots (in milliseconds)</param>
        /// <param name="sector">Definition of area to capture</param>
        /// <returns></returns>
        public static BitmapBatch GetScreenshotBatch(int duration, int frequency, Rectangle sector)
        {
            BitmapBatch batch = new BitmapBatch();
            Stopwatch stopwatch;
            int durationCounter = 0;

            while (durationCounter < duration)
            {
                stopwatch = Stopwatch.StartNew();

                // Caputre screenshot and add to batch
                batch.Add(GetScreenshot(sector));

                // Increment periodCounter
                durationCounter = durationCounter + frequency;

                stopwatch.Stop();

                // Sleep until next iteration
                // (we can afford converting long to int because stopwatch.ElapsedMilliseconds will never be more that INT_MAX)
                Thread.Sleep(frequency - (int)(stopwatch.ElapsedMilliseconds));
            }

            return batch;
        }

        /// <summary>
        /// Captures batch of screenshots of area specified by Rect.
        /// </summary>
        /// <param name="duration">Duration of taking the screenshots (in milliseconds)</param>
        /// <param name="frequency">Frequency of taking the screenshots (in milliseconds)</param>
        /// <param name="sector">Definition of area to capture</param>
        /// <returns></returns>
        public static BitmapBatch GetScreenshotBatch(int duration, int frequency, Rect sector)
        {
            Rectangle rectangleSector = new Rectangle((int)(Math.Floor(sector.Left)), (int)(Math.Floor(sector.Top)), (int)(Math.Floor(sector.Width)), (int)(Math.Floor(sector.Height)));

            return GetScreenshotBatch(duration, frequency, rectangleSector);
        }

        /// <summary>
        /// Captures batch of screenshots of area specified by coordinates and dimensions.
        /// </summary>
        /// <param name="duration">Duration of taking the screenshots (in milliseconds)</param>
        /// <param name="frequency">Frequency of taking the screenshots (in milliseconds)</param>
        /// <param name="top"></param>
        /// <param name="left"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        public static BitmapBatch GetScreenshotBatch(int duration, int frequency, int top, int left, int width, int height)
        {
            return GetScreenshotBatch(duration, frequency, new Rectangle(top, left, width, height));
        }

        /// <summary>
        /// Captures batch of screenshots of whole primary screen.
        /// </summary>
        /// <param name="duration">Duration of taking the screenshots (in milliseconds)</param>
        /// <param name="frequency">Frequency of taking the screenshots (in milliseconds)</param>
        /// <returns></returns>
        public static BitmapBatch GetScreenshotBatch(int duration, int frequency)
        {
            return GetScreenshotBatch(duration, frequency, Screen.PrimaryScreen.Bounds);
        }
    }
}
