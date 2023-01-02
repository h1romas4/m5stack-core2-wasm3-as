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

const DISTANCE: f32 = 5;
const SCALE: f32 = 240;

/**
 * IMU sensor
 */
class Imu {
    private center_x: f32 = 0;
    private center_y: f32 = 0;
    private angle: f32 = 0;

    constructor(
        private width: u32,
        private height: u32
    ) {
        this.init();
    }

    init(): void {
        this.center_x = this.width as f32 / 2.0;
        this.center_y = this.height as f32 / 2.0;
    }

    tick(): void {
        const angle: f32 = this.angle;

        const cos: f32 = Mathf.cos(angle);
        const sin: f32 = Mathf.sin(angle);

        const rot_x: f32[][] = [
            [    1,    0,    0],
            [    0,  cos, -sin],
            [    0,  sin,  cos]
        ];
        const rot_y: f32[][] = [
            [  cos,    0,  -sin],
            [    0,    1,     0],
            [  sin,    0,   cos]
        ];
        const rot_z: f32[][] = [
            [  cos, -sin,     0],
            [  sin,  cos,     0],
            [    0,    0,     1]
        ];

        let proj_2d_points: u32[][] = [];
        let rot_2d: f32[];
        for(let i = 0; i < CUBE_LENGTH; i++) {
            rot_2d = matrix_multiple(rot_y, CUBE_POINTS[i]);
            rot_2d = matrix_multiple(rot_x, rot_2d);
            rot_2d = matrix_multiple(rot_z, rot_2d);
            let z: f32 = 1 / (DISTANCE - rot_2d[2]);
            const proj_2d: f32[] = matrix_multiple(
                [
                    [z, 0, 0],
                    [0, z, 0]
                ],
                rot_2d
            );
            const x = ((proj_2d[0] * SCALE) + this.center_x) as u32;
            const y = ((proj_2d[1] * SCALE) + this.center_y) as u32;
            proj_2d_points[i] = [x, y];
        }

        // Draw
        c3dev.start_write();
        c3dev.fill_rect(0, 0, this.width, this.height, c3dev.COLOR.BLACK);
        for(let i = 0; i < CUBE_LENGTH / 2; i++) {
            this.connect(i, (i + 1) % 4, proj_2d_points);
            this.connect(i + 4, (i + 1) % 4 + 4, proj_2d_points);
            this.connect(i, i + 4, proj_2d_points);
        }
        for(let i = 0; i < CUBE_LENGTH; i++) {
            c3dev.draw_pixel(proj_2d_points[i][0], proj_2d_points[i][1], c3dev.COLOR.RED);
        }
        c3dev.end_write();

        this.angle += 0.01;
    }

    connect(i: u32, j: u32, k: u32[][]): void {
        const a = k[i];
        const b = k[j];
        line(a[0], a[1], b[0], b[1], c3dev.COLOR.BLUE);
    }
}

export function init(width: u32, height: u32): void {
    imu = new Imu(width, height);
}

export function tick(): void {
    if(imu != null) {
        imu!.tick();
    }
}

function matrix_multiple(a: f32[][], b: f32[]): f32[] {
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

function line(x0: u32, y0: u32, x1: u32, y1: u32, color: c3dev.COLOR, tranctl: bool = true): void {
    let dx = abs<i32>(x1 - x0);
    let dy = abs<i32>(y1 - y0);
    let sx = x0 < x1 ? 1: -1;
    let sy = y0 < y1 ? 1: -1;

    let x: i32 = x0;
    let y: i32 = y0;
    let err = dx - dy;

    if(tranctl) c3dev.start_write();

    while(true) {
        c3dev.draw_pixel(x, y, color);
        if(x == x1 && y == y1) {
            break;
        }
        let e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if(e2 < dx) {
            err += dx;
            y += sy;
        }
    }

    if(tranctl) c3dev.end_write();
}
