using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace matrix
{
    /// <summary>
    /// Generic class representing 2D Matrix.
    /// </summary>
    /// <typeparam name="T">Matrix element type.</typeparam>
    public class Matrix<T> where T : IComparable
    {
        private int     matrixWidth;
        private int     matrixHeight;

        private T[,]    matrixData;

        public Matrix(int width, int height, T value) : this(width, height)
        {
            for (int y = 0; y < matrixHeight; ++y)
            {
                for (int x = 0; x < matrixWidth; ++x)
                {
                    matrixData[x, y] = value; 
                }
            }
        }

        public Matrix(int width, int height)
        {
            matrixData = new T[width, height];

            matrixWidth = width;
            matrixHeight = height;
        }

        public T GetElement(int x, int y)
        {
            return matrixData[x, y];
        }

        public void SetElement(int x, int y, T value)
        {
            matrixData[x, y] = value; 
        }

        public T this[int x, int y]
        {
            get { return GetElement(x, y);  }

            set { SetElement(x, y, value);  }
        }

        /// <summary>
        /// Counts occurence of specified value.
        /// </summary>
        /// <param name="value">Value of type T to count.</param>
        /// <returns>Occurence of specified value.</returns>
        public int Count(T value)
        {
            int count = 0;

            IComparable comparableValue = (IComparable)(value);
            IComparable comparableElement;

            for (int y = 0; y < matrixHeight; ++y)
            {
                for (int x = 0; x < matrixWidth; ++x)
                {
                    comparableElement = (IComparable)(matrixData[x, y]);

                    if (matrixData[x, y].CompareTo(value) == 0)
                    {
                        ++count;
                    }
                }
            }

            return count;
        }
    }
}
