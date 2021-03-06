#include <depthos/idt.h>

int __init_idt = 0;

#define idt_size 256
#define count_uintr 129
static struct __idt_entry idt[idt_size];
intr_ht intrs[count_uintr];

struct __idt_ptr idt_ptr;

void idt_init() {
	__init_idt = 1;
	idt_ptr.size = ( sizeof(struct __idt_entry) * idt_size ) - 1;
	idt_ptr.addr = (uint32_t)&idt;
    // Remap the IRQ table
    // Send initialization signal
    outb(0x20,0x11);
    outb(0xA0,0x11);
    // Set offset
    outb(0x21,0x20);
    outb(0xA1,0x28);
    // Set master-slave relation
    outb(0x21,0x04);
    outb(0xA1,0x02);
    // Set 8086 mode
    outb(0x21,0x01);
    outb(0x21,0x01);
    // End of mess
    outb(0x21,0x00);
    outb(0xA1,0x00);

#define idef(i) extern void intr##i(); idt_regh(i,(uint32_t)intr##i)
	idef(0); idef(1); idef(2); idef(3); idef(4); idef(5);
	idef(6); idef(7); idef(8); idef(9); idef(10); idef(11);
	idef(12); idef(13); idef(14); idef(15); idef(16); idef(17);
	idef(18); idef(19); idef(20); idef(21); idef(22); idef(23);
	idef(24); idef(25); idef(26); idef(27); idef(28); idef(29);
	idef(30); idef(31); idef(32); idef(33); idef(34); idef(35);
	idef(36); idef(37); idef(38); idef(39); idef(40); idef(41);
	idef(42); idef(43); idef(44); idef(45); idef(46); idef(47);
	idef(48); idef(49); idef(50); idef(51); idef(52); idef(53);
	idef(54); idef(55); idef(56); idef(57); idef(58); idef(59);
	idef(60); idef(61); idef(62); idef(63); idef(64); idef(65);
	idef(66); idef(67); idef(68); idef(69); idef(70); idef(71);
	idef(72); idef(73); idef(74); idef(75); idef(76); idef(77);
	idef(78); idef(79); idef(80); idef(81); idef(82); idef(83);
	idef(84); idef(85); idef(86); idef(87); idef(88); idef(89);
	idef(90); idef(91); idef(92); idef(93); idef(94); idef(95);
	idef(96); idef(97); idef(98); idef(99); idef(100); idef(101);
	idef(102); idef(103); idef(104); idef(105); idef(106); idef(107);
	idef(108); idef(109); idef(110); idef(111); idef(112); idef(113);
	idef(114); idef(115); idef(116); idef(117); idef(118); idef(119);
	idef(120); idef(121); idef(122); idef(123); idef(124); idef(125);
	idef(126); idef(127); idef(128);
#undef idef
	for ( int i = count_uintr; i < idt_size; i++ ) {
		idt_regh(i,(uint32_t)__idt_default_handler);
	}
	idt_flush();
	print_mod("IDT initialized",MOD_OK);
}

void __idt_default_handler() {
	console_write("hello!");
}


char num_tochar(int i) {
	if ( i == 0 )
	  return '0';
	if ( i == 1 )
	  return '1';	
	if ( i == 2 )
	  return '2';
	if ( i == 3 )
	  return '3';	
	if ( i == 4 )
	  return '4';	
	if ( i == 5 )
	  return '5';	
	if ( i == 6 )
	  return '6';
	if ( i == 7 )
	  return '7';	
	if ( i == 8 )
	  return '8';
	if ( i == 9 )
	  return '9';
	return -1;	
//	for( int i = sizeof(i) / 8; i > 0; i-- ) {
}

void irq_handler(regs_t r) {
	/*console_write("\n");
	console_write("ds:");
	console_write_dec(r.ds);
	console_write("\n");
	console_write("edi:");
	console_write_dec(r.edi);
	console_write("\n");
	console_write("esi:");
	console_write_dec(r.esi);
	console_write("\n");
	console_write("ebp:");
	console_write_dec(r.ebp);
	console_write("\n");
	console_write("esp:");
	console_write_dec(r.esp);
	console_write("\n");
	console_write("ebx:");
	console_write_dec(r.ebx);
	console_write("\n");
	console_write("edx:");
	console_write_dec(r.edx);
	console_write("\n");
	console_write("ecx:");
	console_write_dec(r.ecx);
	console_write("\n");
	console_write("eax:");
	console_write_dec(r.eax);
	console_write("\n");
	console_write("intr num:");
	console_write_dec(r.int_num);
	console_write("\n");
	console_write("err code:");
	console_write_dec(r.err_code);
	console_write("\n");
	console_write("eip:");
	console_write_dec(r.eip);
	console_write("\n");
	console_write("cs:");
	console_write_dec(r.cs);
	console_write("\n");
	console_write("eflags:");
	console_write_dec(r.eflags);
	console_write("\n");
	console_write("user esp:");
	console_write_dec(r.useresp);
	console_write("\n");
	console_write("ss:");
	console_write_dec(r.ss);
	console_write("\n");
	*/

	if ( r.int_num >= 40 ) {
			outb(0xA0,0x20);
	}
	outb(0x20,0x20);

	if ( intrs[r.int_num] != 0 ) {
		intr_ht h = intrs[r.int_num];
		h(r);
	}
//	console_write("irq");
	return;
}


void intr_handler(regs_t r) {
/*	console_write("\n");
	console_write("ds:");
	console_write_dec(r.ds);
	console_write("\n");
	console_write("edi:");
	console_write_dec(r.edi);
	console_write("\n");
	console_write("esi:");
	console_write_dec(r.esi);
	console_write("\n");
	console_write("ebp:");
	console_write_dec(r.ebp);
	console_write("\n");
	console_write("esp:");
	console_write_dec(r.esp);
	console_write("\n");
	console_write("ebx:");
	console_write_dec(r.ebx);
	console_write("\n");
	console_write("edx:");
	console_write_dec(r.edx);
	console_write("\n");
	console_write("ecx:");
	console_write_dec(r.ecx);
	console_write("\n");
	console_write("eax:");
	console_write_dec(r.eax);
	console_write("\n");
	console_write("intr num:");
	console_write_dec(r.int_num);
	console_write("\n");
	console_write("err code:");
	console_write_dec(r.err_code);
	console_write("\n");
	console_write("eip:");
	console_write_dec(r.eip);
	console_write("\n");
	console_write("cs:");
	console_write_dec(r.cs);
	console_write("\n");
	console_write("eflags:");
	console_write_dec(r.eflags);
	console_write("\n");
	console_write("user esp:");
	console_write_dec(r.useresp);
	console_write("\n");
	console_write("ss:");
	console_write_dec(r.ss);
	console_write("\n"); */



	if ( intrs[r.int_num] != 0 ) {
		intr_ht h = intrs[r.int_num];
		h(r);
	}
}


void reg_intr(uint32_t i,intr_ht f) {
	if ( i >= count_uintr ) {
		print_mod("out of range ( interrupt number )", MOD_ERR);
		return;
	}
/*	if (intrs[i] != 0 ) {
		console_write("WARN: not null: register interrupt\n");
	}*/
	intrs[i] = f;
	/*if ( intrs[i] != f ) {
		print_mod("error -> reg interrupt handler",MOD_ERR);
	}
	else {
		print_mod("new interrupt registred",MOD_OK);
	}*/
}

void idt_regh(uint8_t i,uint32_t cb) {
	if ( i >= idt_size) return;
    idt[i].addr_low = (cb & 0x0000ffff);

	idt[i].sel = 0x8;
	idt[i].zero = 0x00;
 	idt[i].flags = 0x8E;
	idt[i].addr_high = ((cb & 0xffff0000) >> 16);
}
