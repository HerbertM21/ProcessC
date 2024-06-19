use std::{mem, os::raw::c_void, panic};

use windows::Win32::{
    Foundation::HANDLE,
    System::{
        Diagnostics::Debug::{ReadProcessMemory, WriteProcessMemory},
        Threading::{OpenProcess, PROCESS_ALL_ACCESS},
    },
};

fn read<T: Default>(handle: HANDLE, pointer: usize) -> T {
    let mut value: T = Default::default();
    let lpbaseaddress = pointer as *const c_void;
    let lpbuffer = &mut value as *mut _ as *mut c_void;
    let nsize = mem::size_of::<T>();

    unsafe {
        let result = ReadProcessMemory(handle, lpbaseaddress, lpbuffer, nsize, None);

        if result.is_err() {
            panic!("Failed to read memory");
        }
    }

    value
}

fn write<T: Default>(handle: HANDLE, pointer: usize, mut data: T) -> bool {
    let lpbaseaddress = pointer as *mut c_void;
    let lpbuffer = &mut data as *mut T as *mut c_void;
    let nsize = mem::size_of::<T>();

    unsafe {
        let result = WriteProcessMemory(handle, lpbaseaddress, lpbuffer, nsize, None);

        if result.is_err() {
            panic!("Failed to write memory");
        }
    }

    true
}

fn main() {
    let pid = 14780; // Dummy program PID
    let pointer = 0xb539b6f43c; // Dummy program pointer
    let handle = unsafe { OpenProcess(PROCESS_ALL_ACCESS, false, pid) };

    // Check if the process is open
    if handle.is_err() {
        println!("Failed to open process");
        return;
    }

    let handle = handle.unwrap();
    let value: i32 = read(handle, pointer);
    println!("Old value: {}", value);

    // Write new value and check if it was written
    write(handle, pointer, 7777 as i32);

    let value: i32 = read(handle, pointer);
    println!("New value: {}", value);
}