import * as c3dev from "./c3dev";

let imu: Imu | null;

const DEG_TO_RAD: f32 = Math.PI / 180;
const CUBE_POINTS: f32[][] = [
    [-1, -1,  1],
    [ 1, -1,  1],
    [ 1,  1,  1],
    [-1,  1,  1],
    [-1, -1, -1],
    [ 1, -1, -1],
    [ 1,  1, -1],
    [-1,  1, -1]
];
const CUBE_LENGTH = 8;

const DISTANCE: f32 = 4;
const SCALE: f32 = 240;

/**
 * IMU sensor
 */
class Imu {
    private centerX: f32 = 0;
    private centerY: f32 = 0;
    private tickAngle: f32 = 0;
    private prevProj2dPoints: u32[][] = [];

    constructor(
        private width: u32,
        private height: u32
    ) {
        this.init();
    }

    init(): void {
        this.centerX = this.width as f32 / 2.0;
        this.centerY = this.height as f32 / 2.0;
        for(let i = 0; i < CUBE_LENGTH; i++) {
            this.prevProj2dPoints[i] = [0, 0];
        }
    }

    rotate(): void {
        const angle: f32 = this.tickAngle;

        const cos: f32 = Mathf.cos(angle);
        const sin: f32 = Mathf.sin(angle);

        const rotX: f32[][] = [
            [    1,    0,     0],
            [    0,  cos,  -sin],
            [    0,  sin,   cos]
        ];
        const rotY: f32[][] = [
            [  cos,    0,  -sin],
            [    0,    1,     0],
            [  sin,    0,   cos]
        ];
        const rotZ: f32[][] = [
            [  cos, -sin,     0],
            [  sin,  cos,     0],
            [    0,    0,     1]
        ];

        this.draw(rotX, rotY, rotZ);

        this.tickAngle += 0.04;
    }

    angle(roll: f32, pitch: f32, yaw: f32): void {
        const cosX: f32 = Mathf.cos(roll * DEG_TO_RAD);
        const sinX: f32 = Mathf.sin(roll * DEG_TO_RAD);
        const cosY: f32 = Mathf.cos(pitch * DEG_TO_RAD);
        const sinY: f32 = Mathf.sin(pitch * DEG_TO_RAD);

        const cos: f32 = Mathf.cos(0);
        const sin: f32 = Mathf.sin(0);

        const rotX: f32[][] = [
            [     1,     0,      0],
            [     0,  cosX,  -sinX],
            [     0,  sinX,   cosX]
        ];
        const rotY: f32[][] = [
            [  cosY,     0,  -sinY],
            [      0,    1,      0],
            [  sinY,     0,   cosY]
        ];
        const rotZ: f32[][] = [
            [    cos,  -sin,     0],
            [    sin,   cos,     0],
            [      0,     0,     1]
        ];

        this.draw(rotX, rotY, rotZ);
    }

    draw(rotX: f32[][], rotY: f32[][], rotZ: f32[][]): void {
        let proj2dPoints: u32[][] = [];
        let rot2d: f32[];
        for(let i = 0; i < CUBE_LENGTH; i++) {
            rot2d = matrixMultiple(rotY, CUBE_POINTS[i]);
            rot2d = matrixMultiple(rotX, rot2d);
            rot2d = matrixMultiple(rotZ, rot2d);
            let z: f32 = 1 / (DISTANCE - rot2d[2]);
            const proj2d: f32[] = matrixMultiple(
                [
                    [z, 0, 0],
                    [0, z, 0]
                ],
                rot2d
            );
            const x = ((proj2d[0] * SCALE) + this.centerX) as u32;
            const y = ((proj2d[1] * SCALE) + this.centerY) as u32;
            proj2dPoints[i] = [x, y];
        }

        // c3dev.fill_rect(0, 0, this.width, this.height, c3dev.COLOR.BLACK);
        c3dev.start_write();
        // clear prev lines
        for(let i = 0; i < CUBE_LENGTH / 2; i++) {
            this.connect(i, (i + 1) % 4, this.prevProj2dPoints, c3dev.COLOR.BLACK);
            this.connect(i + 4, (i + 1) % 4 + 4, this.prevProj2dPoints, c3dev.COLOR.BLACK);
            this.connect(i, i + 4, this.prevProj2dPoints, c3dev.COLOR.BLACK);
        }
        // draw lines
        for(let i = 0; i < CUBE_LENGTH / 2; i++) {
            this.connect(i, (i + 1) % 4, proj2dPoints, c3dev.COLOR.GREEN);
            this.connect(i + 4, (i + 1) % 4 + 4, proj2dPoints, c3dev.COLOR.GREEN);
            this.connect(i, i + 4, proj2dPoints, c3dev.COLOR.GREEN);
        }
        // draw points and save prev
        for(let i = 0; i < CUBE_LENGTH; i++) {
            const x = proj2dPoints[i][0];
            const y = proj2dPoints[i][1];
            c3dev.draw_pixel(x, y, c3dev.COLOR.RED);
            this.prevProj2dPoints[i] = [x, y];
        }
        c3dev.end_write();
    }

    connect(i: u32, j: u32, k: u32[][], color: c3dev.COLOR): void {
        const a = k[i];
        const b = k[j];
        c3dev.draw_line(a[0], a[1], b[0], b[1], color);
    }
}

export function init(width: u32, height: u32): void {
    imu = new Imu(width, height);
}

export function rotate(): void {
    if(imu != null) {
        imu!.rotate();
    }
}

export function angle(roll: f32, pitch: f32, yaw: f32): void {
    if(imu != null) {
        imu!.angle(roll, pitch, yaw);
    }
}

function matrixMultiple(a: f32[][], b: f32[]): f32[] {
    let result: f32[] = [];

    for(let i = 0; i < a.length; i++) {
        let sum: f32 = 0;
        for(let j = 0; j < 3; j++) {
            sum += a[i][j] * b[j];
        }
        result[i] = sum;
    }

    return result;
}
