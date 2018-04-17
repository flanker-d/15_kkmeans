#include <iostream>
#include <vector>

#include <dlib/clustering.h>
#include <dlib/rand.h>
#include <dlib/image_io.h>
#include <cmath>

#include "points.h"

using namespace std;
using namespace dlib;

using sample_type = matrix<double,2,1>;
using kernel_type = radial_basis_kernel<sample_type>;


class color_storage
{
  public:
    color_storage(int clusters_count)
    {
      int colors_count = clusters_count / 3;
      int base_r = 0x00;
      int base_g = 0x00;
      int base_b = 0x00;

      int divisor= std::cbrt(clusters_count) + 1.0; /* cube root */
      std::uint8_t step_r=(255 - base_r) / (int)divisor;
      std::uint8_t step_g=(255 - base_g) / (int)divisor;
      std::uint8_t step_b=(255 - base_b) / (int)divisor;
      for(std::uint8_t r=255; r >= base_r + step_r; r-=step_r)
      {
        for(std::uint8_t g=255; g >= base_g + step_g; g-=step_g)
        {
          for(std::uint8_t b=255; b >= base_b + step_b; b-=step_b)
          {
            m_colors.push_back({r,g,b});
          }
        }
      }
    }

    rgb_pixel get_color(int cluster_number)
    {
      return m_colors.at(cluster_number);
    }
  private:
    std::vector<rgb_pixel> m_colors;
};


int main(int argc, char **argv)
{
  //if(argc == 2)
  {
    //int clusters_num = std::stoi(argv[1]);
    int clusters_num = 5;

    std::vector<sample_type> points;
    sample_type m;
    for(int i = 0; i < data_set.size(); i++)
    {
      m(0) = data_set.at(i).first;
      m(1) = data_set.at(i).second;
      points.push_back(m);
    }

    kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);
    kkmeans<kernel_type> test(kc);
    test.set_number_of_centers(clusters_num);
    std::vector<sample_type> initial_centers;
    pick_initial_centers(clusters_num, initial_centers, points, test.get_kernel());
    test.train(points, initial_centers);

    std::map<int, std::vector<sample_type>> clustered_map;
    for (unsigned long i = 0; i < points.size(); i++)
    {
      int c = test(points[i]);
      clustered_map[c].push_back(points[i]);
    }
    cout << endl;

    color_storage cs(clusters_num);
    array2d<rgb_pixel> img(200, 200);
    int i = 0;
    for(auto c : clustered_map)
    {
      int cluster = c.first;
      for(auto point : c.second)
      {
        int x = point(0);
        int y = point(1);
        img[x][y] = cs.get_color(cluster);
        i++;
      }
    }
    dlib::save_png(img, "./kmeans.png");
  }


  return 0;
}
