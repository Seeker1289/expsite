const dict = ['abacus', 'abandon', 'abbreviate', 'abbreviation', 'abdomen', 'abduct', 'abide', 'ability', 'able', 'abnormal', 'abolish', 'abort', 'about', 'above', 'abroad', 'abrupt', 'abs', 'abscissa', 'absent', 'absentee', 'absolute', 'absorb', 'absorbent', 'abstain', 'abstract', 'absurd', 'abundant', 'abuse', 'abyss', 'acacia', 'academia', 'academy', 'accelerate', 'accelerator', 'accent', 'accept', 'access', 'accession', 'accessory', 'accident', 'accommodate', 'accompany', 'accomplish', 'accomplishment', 'accord', 'account', 'accountable', 'accountant', 'accredit', 'accumulate', 'accuracy', 'accurate', 'accuse', 'accustom', 'ace', 'ache', 'achieve', 'achievement', 'achiever', 'acid', 'acknowledge', 'acknowledgement', 'acne', 'acorn', 'acoustic', 'acoustics', 'acquaint', 'acquaintance', 'acquire', 'acre', 'acrobat', 'acrobatics', 'acronym', 'across', 'acrylic', 'act', 'action', 'activate', 'active', 'activist', 'activity', 'actor', 'actress', 'actual', 'actualize', 'actuary', 'actuate', 'actuator', 'acupressure', 'acupuncture', 'acute', 'adagio', 'adamant', 'adapt', 'adaptable', 'adaptation', 'add', 'addict', 'addiction', 'addition', 'additional', 'additive', 'address', 'adequate', 'adhere', 'adhesive', 'adieu', 'adjacent', 'adjective', 'adjoin', 'adjust', 'adjustable', 'adjustment', 'administer', 'administrate', 'administration', 'admire', 'admission', 'admit'];
let rand = Math.floor(Math.random()*dict.length);
let word = dict[rand];
/*let word = prompt('word');*/
let blanks = document.querySelector("h1");
const button = document.querySelector("button");
let gss = document.querySelector("#gss");
let a ='';
let b = [];
let count;
let wrongs = 0;
let check;
let n;
let l = word.length;
let letter;
let comma = 0;
document.querySelector('h3').innerText = `It is a ${l} letter word.`;
for (let i of word) {
    b.push("-");
    a = a + '-';
    blanks.innerText = a;
}
/*button.onclick = () => {blanks.innerText = document.querySelector('input').value;};*/
button.onclick = function() {
    letter = document.querySelector('input').value.toLowerCase();
    if (letter.length == 1) {
        wrongs++;
        a = '';
        count = 0;
        check = 0;
        n = 0;
        for (i=0; i<l; i++) {
            if (word[i] == letter) {
                b[i] = letter;
                a = a + b[i];
                wrongs--;
                if (check != 0) {
                    wrongs++;
                }
                check++;
            } else {
                a = a + b[i];
                n++;
            }
        }
        for (i=0; i<l; i++) {
            if (b[i] != '-') {
                count++;
            }
        }
        if (n == l) {
            document.querySelector('h3').innerText = `${letter} is  not there.`;
            if (comma != 0) {
                gss.innerText =  gss.innerText + ',' + letter;
            } else {
                gss.innerText = letter;
                comma++;
            }
        } else {
            document.querySelector('h3').innerText = '';
        }
        if (count == l) {
            document.querySelector('h3').innerText = `You guessed it.`;
        }
        document.querySelector('h4').innerText = `${8-wrongs} chances remaining`;
        blanks.innerText = a;
        if (wrongs > 7) {
            document.querySelector('h3').innerText = `You failed to guess it.`;
            blanks.innerText = word;
        }
    } else {
        document.querySelector('h3').innerText = 'Give a valid input. It should contain one letter.'
    }
}
