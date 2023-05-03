#ifndef AL_IRQ_H
#define AL_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef void (*AL_isr_handler_t)(int vector, void *param);

struct AL_irq_desc
{
    AL_isr_handler_t 	handler;
    void            	*param;

#ifdef AL_USING_INTERRUPT_INFO
    char             name[AL_NAME_MAX];
    AL_uint32_t      counter;
#endif
};


/*
 * Interrupt interfaces
 */
void AL_hw_interrupt_init(void);

void AL_hw_interrupt_mask(int vector);

void AL_hw_interrupt_umask(int vector);

AL_isr_handler_t AL_hw_interrupt_install(int              vector,
                                         AL_isr_handler_t handler,
                                         void            *param,
                                         const char      *name);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_IRQ_H */