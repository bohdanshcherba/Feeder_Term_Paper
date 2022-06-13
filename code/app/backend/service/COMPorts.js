import {SerialPort, ReadlineParser} from "serialport";
import {insert} from './db.js'
const port = new SerialPort({
    path: 'COM4',
    baudRate: 9600,
})

let onetime = true;

const parser = new ReadlineParser()
port.pipe(parser)
let current_weight = 0
let loaded_weight = 0
let eat_weight = 0
let value = 0
let is_loading_food = 0
let is_eating = 0
let id_timeout1 = 0
let id_timeout2 = 0
const d = new Date();

parser.on("data", (line)=>{
    value = line
    console.log(value)
    if (value > current_weight){
        current_weight = value
        is_loading_food = 1
        if(id_timeout1 !== 0){
            clearTimeout(id_timeout1)
        }
        id_timeout1 = setTimeout(function(){
            is_loading_food = -1;
            loaded_weight = current_weight
        }, 3000);
    }
    else if(value < current_weight) {
        current_weight = value
        is_eating = 1
        if(id_timeout2 !== 0){
            clearTimeout(id_timeout2)
        }
        id_timeout2 = setTimeout(function(){
            is_eating = -1;
            eat_weight = loaded_weight - current_weight
            loaded_weight = current_weight

            const d = new Date();

            const yyyy = d.getFullYear();
            let mm = d.getMonth() + 1; // Months start at 0!
            let dd = d.getDate();

            if (dd < 10) dd = '0' + dd;
            if (mm < 10) mm = '0' + mm;

            let today = dd + '/' + mm + '/' + yyyy;
            let hour = d.getHours();
            let minutes = d.getMinutes();
            if(eat_weight>0){
                insert(`${today} ${hour}:${minutes}`, Math.round(eat_weight * 100000) / 100)

            }
        }, 5000);
    }
})


let hour = d.getHours();
let minutes = d.getMinutes();

if (onetime){
    onetime= false
    port.write(`2,${hour},${minutes};`)
}

const read = () => {
  return value
}

const write = (data) => {
    port.write(data)
}

export {
    read,
    write,
}
