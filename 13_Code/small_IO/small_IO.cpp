#include <omp.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>

using namespace std;
namespace fs = filesystem;

vector<string> ls_files_recursive(const string& path) {
  vector<string> files;
  files.reserve(30000);
  for (auto& p : fs::recursive_directory_iterator(path)) {
    if (p.is_regular_file()) {
      const auto filename = p.path().string();
      files.push_back(filename);
    }
  }
  return files;
}

string get_title(const string& file) {
  const static string tag = "<page title>\": ";
  ifstream fin(file);
  string line;
  while (getline(fin, line)) {
    auto position = line.find(tag);
    if (position != string::npos) {
      position = position + tag.size();
      // extract string between ': "' and '",' quotes are included
      line = line.substr(position, line.size() - position - 1);
      return line;
    }
  }
  return "NO_TITLE";
}

// extract titles from different files
// "vector<string> &files" contains the paths to the files
vector<string> get_titles(const vector<string>& files, int threads) {
  vector<string> titles(files.size());
#pragma omp parallel for schedule(dynamic, 8) num_threads(threads)
  for (size_t i = 0; i < files.size(); ++i) {
    // function get_title opens file and extracts the title
    titles[i] = get_title(files[i]);
  }
  return titles;  // return a vector of titles
}

// before running on linux "sync; echo 3 > /proc/sys/vm/drop_caches"
// --> clears the page cache, dentries, and inodes
// on OSX "sync && sudo purge"
int main() {
  const string camera_specs_dir = "2013_camera_specs";
  int threads = 16;

  double tic = omp_get_wtime();
  vector<string> files = ls_files_recursive(camera_specs_dir);
  double time_in_s_ls = omp_get_wtime() - tic;

  tic = omp_get_wtime();
  auto titles = get_titles(files, threads);
  double time_in_s_get_titles = omp_get_wtime() - tic;

  cout << "ls_files_recursive: " << time_in_s_ls << " s" << endl;
  cout << "get_titles: " << time_in_s_get_titles << " s" << endl;
  cout << "total: " << time_in_s_ls + time_in_s_get_titles << " s" << endl;
  cout << "titles count: " << titles.size() << endl;
}
