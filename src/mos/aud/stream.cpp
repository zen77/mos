#include <mos/aud/stream.hpp>

namespace mos {
namespace aud {

std::atomic_uint Stream::current_id_;

Stream::Stream(const std::string &path)
    : file_name_(path), id_(current_id_++) {
  vorbis_stream_ = stb_vorbis_open_filename((char *) path.c_str(), NULL, NULL);
  vorbis_info_ = stb_vorbis_get_info(vorbis_stream_);
  samples_left_ = stb_vorbis_stream_length_in_samples(vorbis_stream_) *
      vorbis_info_.channels;
}

Stream::~Stream() { stb_vorbis_close(vorbis_stream_); }

std::array<short, Stream::buffer_size> Stream::read() {
  auto samples = std::array<short, Stream::buffer_size>();

  int size = 0;
  int result = 0;

  while (size < Stream::buffer_size) {
    result = stb_vorbis_get_samples_short_interleaved(
        vorbis_stream_, vorbis_info_.channels, samples.data() + size,
        buffer_size - size);
    if (result > 0)
      size += result * vorbis_info_.channels;
    else
      break;
  }
  samples_left_ -= size;
  return samples;
}

bool Stream::done() const { return samples_left_ <= 0; }

int Stream::sample_rate() const { return vorbis_info_.sample_rate; }

int Stream::channels() const { return vorbis_info_.channels; }

void Stream::seek_start() {
  stb_vorbis_seek_start(vorbis_stream_);
  samples_left_ = size() * channels();
}

unsigned int Stream::id() const { return id_; }

float Stream::duration() const {
  return stb_vorbis_stream_length_in_seconds(vorbis_stream_);
}
size_t Stream::size() const {
  return stb_vorbis_stream_length_in_samples(vorbis_stream_);
}
}
}
