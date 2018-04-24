#include <vector>
#include <string>
#include <dlib/clustering.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

using sample_type = dlib::matrix<double, 2, 1>;
using kernel_type = dlib::radial_basis_kernel<sample_type>;

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    unsigned long clusters_num = std::stoul(argv[1]);

    std::vector<sample_type> points;
    sample_type m;
    std::string point_coord;
    std::string coord;
    while(std::getline(std::cin, point_coord))
    {
      std::stringstream input(point_coord);
      for(int i = 0; i < 2; i++)
      {
        std::getline(input, coord, ';');
        m(i) = std::stod(coord);
      }
      points.push_back(m);
    }

    dlib::kcentroid<kernel_type> kc(kernel_type(0.0001), 0.001, clusters_num);
    dlib::kkmeans<kernel_type> test(kc);
    test.set_number_of_centers(clusters_num);
    std::vector<sample_type> initial_centers;
    pick_initial_centers(clusters_num, initial_centers, points, test.get_kernel());
    test.train(points, initial_centers);

    std::map<int, std::vector<sample_type>> clustered_map;
    for (const auto &point : points)
    {
      unsigned long c = test(point);
      clustered_map[c].push_back(point);
    }

    dlib::array2d<dlib::rgb_pixel> img(200, 200);
    for(int i = 0; i < 200; i++)
      for(int j = 0; j < 200; j++)
        img[i][j] = dlib::rgb_pixel(255, 255, 255);

    int i = 0;
    for(auto c : clustered_map)
    {
      int cluster = c.first;
      for(auto point : c.second)
      {
        double x = point(0) + 100;
        double y = point(1) + 100;
        img[x][y] = dlib::colormap_jet(cluster, 0, clusters_num);
        i++;
      }
    }
    dlib::save_png(img, "./kmeans.png");
  }

  return 0;
}
