# syntax=docker/dockerfile:1.7

FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    ca-certificates \
    curl \
    unzip \
    zip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

FROM base AS builder

COPY . /app

RUN cmake -S . -B build -DBUILD_TESTING=ON \
 && cmake --build build -j"$(nproc)"

FROM builder AS tester

RUN ctest --test-dir build --output-on-failure

FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY --from=builder /app/build/docgen /usr/local/bin/docgen
COPY --from=builder /app/assets /opt/docgen-assets

ENTRYPOINT ["/usr/local/bin/docgen"]
CMD ["-i", "/workspace/src", "-o", "/workspace/build/docs", "--exclude", ".git,build,out"]
