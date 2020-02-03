#include "headers.h"

int main(int argc, char const *argv[]) {

	try {

		std::ifstream input(argv[1], std::ios::in | std::ios::binary);
		if(!input.is_open())
			throw std::runtime_error("Error! Unable to open file.\n");

		input.seekg(0, input.end);
		const size_t len = input.tellg();
		input.seekg(0, input.beg);

		auto tmp_buf = std::make_unique<char[]>(len);
		input.read(tmp_buf.get(), len);
		input.close();
		
		std::vector<std::complex<double>> data;
		data.reserve(len / 2);
		for(size_t i{0}; i < len -1 ; ++i)
			data.push_back({static_cast<double>(*((uint8_t*)tmp_buf.get() + i) - 127.5),
					static_cast<double>(*((uint8_t*)tmp_buf.get() + i + 1) - 127.5)});

		fftw_plan plan = fftw_plan_dft_1d(data.size(),
						  (fftw_complex*) &data[0],
						  (fftw_complex*) &data[0],
						  FFTW_FORWARD,
						  FFTW_ESTIMATE);

		fftw_execute(plan);
		fftw_destroy_plan(plan);

		//std::ofstream out("spectr.txt");
		for(const auto& i: data)
			std::cout << i << '\n';

	}
	catch(const std::exception& error) {
		std::cerr << error.what();
	}

	return 0;
}
