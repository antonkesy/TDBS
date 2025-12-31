FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    clang \
    make \
    git \
    # GTest
    libgtest-dev \
    # raylib dependencies
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libwayland-dev \
    libxkbcommon-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make config build

FROM ubuntu:24.04 AS runtime
ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies for raylib
RUN apt-get update && apt-get install -y \
    libasound2t64 \
    libx11-6 \
    libxrandr2 \
    libxi6 \
    libgl1 \
    libglu1-mesa \
    libxcursor1 \
    libxinerama1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built executable from builder stage
COPY --from=builder /app/build/tdbs .

CMD ["./tdbs"]
